package at.kingcastle;

import gnu.io.*;
import org.apache.commons.compress.compressors.CompressorInputStream;
import org.apache.commons.compress.compressors.CompressorOutputStream;
import org.apache.commons.compress.compressors.CompressorStreamFactory;
import org.apache.commons.compress.compressors.gzip.GzipCompressorInputStream;
import org.apache.commons.compress.utils.IOUtils;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Arrays;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\goldenaxetitle.vgm");
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\dragonthroatcave.vgm");
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\boss1.vgm");
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\angelisland.vgm");
    public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\greenhill1.vgm");
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\island.vgm");
    public static File dir = new File("C:\\Users\\kingc\\Dropbox\\ym2612\\Sonic the Hedgehog");
    //public static File dir = new File("C:\\Users\\kingc\\Dropbox\\ym2612\\Sonic the Hedgehog 2");
    //public static File dir = new File("C:\\Users\\kingc\\Dropbox\\ym2612\\Sonic the Hedgehog 3 & Knuckles");
    //public static File dir = new File("C:\\Users\\kingc\\Dropbox\\ym2612\\Golden Axe II");
    //public static File dir = new File("C:\\Users\\kingc\\Dropbox\\ym2612\\Ecco the Dolphin");

    public static InputStream in;
    public static OutputStream out;
    public static Lock lock = new ReentrantLock();
    public static Condition notFinished = lock.newCondition();

    public static volatile boolean stop_sending = false;

    public static class SerialReader implements SerialPortEventListener {
        private InputStream in;

        public SerialReader(InputStream in) {
            this.in = in;
        }

        public void serialEvent(SerialPortEvent arg0) {
            int data;
            try {
                StringBuffer strBuff = new StringBuffer();
                while ((data = in.read()) > -1) {
                    strBuff.append((char) data);
                    if (data == '\n') {
                        String receivedStr = strBuff.toString();
                        System.out.print("teensy: " + receivedStr);
                        if (receivedStr.contains("end")) {
                            lock.lock();
                            notFinished.signalAll();
                            lock.unlock();
                        }
                        strBuff = new StringBuffer();
                    }
                }
                System.out.println(strBuff.toString());
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static class SerialWriter implements Runnable {
        OutputStream out;

        public SerialWriter(OutputStream out) {
            this.out = out;
        }

        public void run() {
            try {
                while (true) {
                    File[] playlist = dir.listFiles(new FilenameFilter() {
                        public boolean accept(File dir, String name) {
                            if (name.endsWith(".vgz")) {
                                return true;
                            }
                            return false;
                        }
                    });
                    File[] playlist2 = new File[]{playlist[1]};
                    //File[] playlist2 = new File[playlist.length - 0];
                    //playlist2 = Arrays.copyOfRange(playlist, 0, playlist2.length);
                    for (File song : playlist2) {
                        System.out.println("playing " + song.getName());
                        BufferedInputStream in = new BufferedInputStream(new FileInputStream(song));
                        GzipCompressorInputStream gzIn = new GzipCompressorInputStream(in);
                        expandVGM(gzIn, out);
                        gzIn.close();
                        lock.lock();
                        notFinished.await();
                        lock.unlock();
                        Thread.sleep(1000);
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            } finally {
                lock.unlock();
            }
        }
    }

    public static void main(String[] args) {
        try {
            final String port = "COM4";
            int baudRate = 1000000;
            java.util.Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
            CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(port);
            if (portIdentifier.isCurrentlyOwned()) {
                System.out.println("Error: Port is currently in use");
            } else {
                CommPort commPort = portIdentifier.open("vgmread", 2000);

                if (commPort instanceof SerialPort) {
                    SerialPort serialPort = (SerialPort) commPort;
                    System.out.println("port: " + port + " baudRate: " + baudRate);
                    serialPort.setSerialPortParams(baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                    in = serialPort.getInputStream();
                    out = serialPort.getOutputStream();

                    serialPort.addEventListener(new SerialReader(in));
                    serialPort.notifyOnDataAvailable(true);
                    serialPort.notifyOnOverrunError(true);

                    //Thread.sleep(500); // goood lords

                    (new Thread(new SerialWriter(out))).start();
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void expandVGM(InputStream is, OutputStream out) throws IOException {
        byte[] music = IOUtils.toByteArray(is);

        //ident check
        if (!(music[0] == 0x56 && music[1] == 0x67 && music[2] == 0x6d && music[3] == 0x20)) {
            System.out.println("ident failed!");
            return;
        }

        //version
        int version = ((music[8] << 0) & 0x000000FF) | ((music[9] << 8) & 0x0000FF00)
                | ((music[10] << 16) & 0x00FF0000) | ((music[11] << 24) & 0xFF000000);
        System.out.println(BCDtoString(music[11]) + "." + BCDtoString(music[10]) + "." + BCDtoString(music[9]) + "." + BCDtoString(music[8]));

        //total samples
        int samples = ((music[24] << 0) & 0x000000FF) | ((music[25] << 8) & 0x0000FF00)
                | ((music[26] << 16) & 0x00FF0000) | ((music[27] << 24) & 0xFF000000);
        System.out.println("samples: " + samples);

        //loop offset
        int loopOffset = ((music[28] << 0) & 0x000000FF) | ((music[29] << 8) & 0x0000FF00)
                | ((music[30] << 16) & 0x00FF0000) | ((music[31] << 24) & 0xFF000000);
        System.out.println("loopOffset: " + loopOffset);
        loopOffset += 0x1c; // make relative offset an absolute offset

        //loop samples
        int loopSamples = ((music[32] << 0) & 0x000000FF) | ((music[33] << 8) & 0x0000FF00)
                | ((music[34] << 16) & 0x00FF0000) | ((music[35] << 24) & 0xFF000000);
        System.out.println("loopSamples: " + loopSamples);

        //data offset
        int vgmDataOffset = ((music[52] << 0) & 0x000000FF) | ((music[53] << 8) & 0x0000FF00)
                | ((music[54] << 16) & 0x00FF0000) | ((music[55] << 24) & 0xFF000000);
        System.out.println("vgmDataOffset: " + vgmDataOffset);
        vgmDataOffset += 0x34; // make relative offset an absolute offset

        out.write(music, 0, 64);
        ByteArrayOutputStream bos = new ByteArrayOutputStream();

        long startTime = System.currentTimeMillis();
        long samplesSum = 0;
        int datablockOffset = 0;
        int datablockIndex = 0;
        for (int i = 64; i < music.length; i++) {
            long waitSamples = 0;
            if (music[i] == (byte) 0x4F) {
                //0x4F dd    : Game Gear PSG stereo, write dd to port 0x06
                i += 1;
            } else if (music[i] == (byte) 0x52) {
                //0x52 aa dd : YM2612 port 0, write value dd to register aa
                //write_data(music[i + 1], music[i + 2], PORT_0);
                if(music[i + 1] == (byte)0x40 || music[i + 1] == (byte)0x44 || music[i + 1] == (byte)0x48) {
                    System.out.printf("PORT_0: %x %x\n", music[i + 1], music[i + 2]);
                    music[i + 2] = 120;
                }
                bos.write(music, i, 3);
                i += 2;
            } else if (music[i] == (byte) 0x53) {
                //0x53 aa dd : YM2612 port 1, write value dd to register aa
                //write_data(music[i + 1], music[i + 2], PORT_1);
                //bos.write(music, i, 3);
                if(music[i + 1] == (byte)0x40 || music[i + 1] == (byte)0x44 || music[i + 1] == (byte)0x48) {
                    System.out.printf("PORT_1: %x %x\n", music[i + 1], music[i + 2]);
                    music[i + 2] = 120;
                }
                i += 2;
            } else if (music[i] == (byte) 0x50) {
                //0x50	dd	PSG (SN76489/SN76496) write value dd
                // skip
                i += 1;
            } else if (music[i] == (byte) 0x61) {
                //0x61 nn nn : Wait n samples, n can range from 0 to 65535 (approx 1.49 seconds). Longer pauses than this are represented by multiple wait commands.
                waitSamples = ((music[i + 1] << 0) & 0x000000FF) | ((music[i + 2] << 8) & 0x0000FF00);
                samplesSum += waitSamples;
                bos.write(music, i, 3);
                i += 2;
            } else if (music[i] == (byte) 0x62) {
                //0x62		wait 735 samples (60th of a second), a shortcut for 0x61 0xdf 0x02
                waitSamples = 735;
                samplesSum += waitSamples;
                bos.write(music, i, 1);
            } else if (music[i] == (byte) 0x63) {
                //0x63		wait 882 samples (50th of a second), a shortcut for 0x61 0x72 0x03
                waitSamples = 882;
                samplesSum += waitSamples;
                bos.write(music, i, 1);
            } else if (music[i] >= (byte) 0x70 && music[i] <= (byte) 0x7f) {
                //0x7n       : wait n+1 samples, n can range from 0 to 15
                waitSamples = 1 + (music[i] & 0x0f);
                samplesSum += waitSamples;
                bos.write(music, i, 1);
            } else if (music[i] >= (byte) 0x80 && music[i] <= (byte) 0x8f) {
                //YM2612 port 0 address 2A write from the data bank, then wait n samples;
                waitSamples = music[i] & 0x0f;
                samplesSum += waitSamples;
                //System.out.printf("0x8n index: %d write: 0x%x wait: %d %dys %d\n", datablockIndex, music[datablockOffset + datablockIndex], waitSamples, (waitSamples * 1000000) / 44100, i);
                bos.write(music, i, 1);
                bos.write(music, datablockOffset + datablockIndex, 1);
                datablockIndex++;
            } else if (music[i] == (byte) 0x67) {
                //data block 0x67 0x66 tt ss ss ss ss (data)
                int data_type = music[i + 2];
                int size = ((music[i + 3] << 0) & 0x000000FF) | ((music[i + 4] << 8) & 0x0000FF00)
                        | ((music[i + 5] << 16) & 0x00FF0000) | ((music[i + 6] << 24) & 0xFF000000);
                //System.out.printf("data block, type: %d, size: %d\n", data_type, size);
                datablockOffset = i + 7;
                i += 6 + size;
            } else if (music[i] == (byte) 0xE0) {
                //0xE0	dddddddd	Seek to offset dddddddd (Intel byte order) in PCM data bank of data block type 0 (YM2612).
                int offset = ((music[i + 1] << 0) & 0x000000FF) | ((music[i + 2] << 8) & 0x0000FF00)
                        | ((music[i + 3] << 16) & 0x00FF0000) | ((music[i + 4] << 24) & 0xFF000000);
                datablockIndex = offset;
                //System.out.printf("PCM data, offset: %d\n", offset);
                i += 4;
            } else if (music[i] == (byte) 0x66) {
                //0x66       : end of sound data
                if (loopOffset != 0) {
                    i = loopOffset - 1;
                    continue;
                }
                bos.write(music, i, 1);
                break;
            } else {
                System.out.printf("unbekanntes zeichen 0x%x pos:%d\n", music[i], i);
                break;
            }

            if (bos.size() >= 1024) {
                byte[] packet = bos.toByteArray();
                out.write(packet);
                bos.reset();
            }
        }
        byte[] packet = bos.toByteArray();
        out.write(packet);
        System.out.printf("sent samples: %d ms: %d\n", samplesSum, (samplesSum * 1000) / 44100);
    }

    public static void busyWaitMicros(long micros) {
        long waitUntil = System.nanoTime() + (micros * 1000);
        while (waitUntil > System.nanoTime()) {
            ;
        }
    }

    public static String BCDtoString(byte bcd) {
        StringBuffer sb = new StringBuffer();

        byte high = (byte) (bcd & 0xf0);
        high >>>= (byte) 4;
        high = (byte) (high & 0x0f);
        byte low = (byte) (bcd & 0x0f);

        sb.append(high);
        sb.append(low);

        return sb.toString();
    }
}