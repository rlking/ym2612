import com.sun.xml.internal.ws.encoding.MtomCodec;
import gnu.io.*;

import java.io.*;
import java.nio.ByteBuffer;
import java.util.stream.IntStream;

public class Main {
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\goldenaxetitle.vgm");
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\angelisland.vgm");
    public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\greenhill1.vgm");
    //public static File vgm = new File("C:\\Users\\kingc\\workspace\\ym2612\\island.vgm");
    public static byte[] data;
    public static int sent;

    public static InputStream in;
    public static OutputStream out;

    public static volatile boolean stop_sending = false;

    /**
     * Handles the input coming from the serial port. A new line character
     * is treated as the end of a block in this example.
     */
    public static class SerialReader implements SerialPortEventListener {
        private InputStream in;
        private byte[] buffer = new byte[1024];

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
                        System.out.print(strBuff.toString());
                        strBuff = new StringBuffer();
                    }
                }
                System.out.println(strBuff.toString());
            } catch (IOException e) {
                e.printStackTrace();
                System.exit(-1);
            }
        }
    }

    public static class SerialWriter implements Runnable {
        OutputStream out;
        long time = System.currentTimeMillis();

        public SerialWriter(OutputStream out) {
            this.out = out;
        }

        public void run() {
            try {
                ByteBuffer ack = ByteBuffer.allocate(8);
                ack.put((byte) 'a');
                ack.put((byte) 'c');
                ack.put((byte) 'k');
                ack.put((byte) 'n');
                ack.putInt(data.length);
                out.write(ack.array());
                //Thread.sleep(300);
                while (sent < data.length) {
                    if (stop_sending) {
                        continue;
                    }
                    //System.out.println("sent data[" + sent + "] delay: " + (System.currentTimeMillis() - time));
                    int size = 64;
                    time = System.currentTimeMillis();
                    out.write(data, sent, data.length - sent < size ? data.length - sent : size);
                    sent += size;
                    //Thread.sleep(10);
                }
                System.out.println("wrote: " + data.length);
                Thread.sleep(5000);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public static void main(String[] args) {
        data = new byte[(int) vgm.length()];

        try {
            FileInputStream fis = new FileInputStream(vgm);
            fis.read(data, 0, data.length);
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }

        readVGM(vgm);

        try {
            final String port = "COM4";
            int baudRate = 1000000;
            java.util.Enumeration<CommPortIdentifier> portEnum = CommPortIdentifier.getPortIdentifiers();
            while ( portEnum.hasMoreElements() )
            {
                CommPortIdentifier portIdentifier = portEnum.nextElement();
                System.out.println(portIdentifier.getName()  +  " - " +  getPortTypeName(portIdentifier.getPortType()) );
            }
            CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(port);
            if (portIdentifier.isCurrentlyOwned()) {
                System.out.println("Error: Port is currently in use");
            } else {
                CommPort commPort = portIdentifier.open("vgmread", 2000);

                if (commPort instanceof SerialPort) {
                    SerialPort serialPort = (SerialPort) commPort;
                    if (port.equals("COM5")) {
                        baudRate = 115200;
                    } else {
                        baudRate = 1000000;
                    }
                    System.out.println("port: " + port + " baudRate: " + baudRate);
                    serialPort.setSerialPortParams(baudRate, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                    in = serialPort.getInputStream();
                    out = serialPort.getOutputStream();

                    serialPort.addEventListener(new SerialReader(in));
                    serialPort.notifyOnDataAvailable(true);
                    serialPort.notifyOnOverrunError(true);

                    Thread.sleep(500); // goood lords

                    (new Thread(new SerialWriter(out))).start();
                }
            }
        } catch (
                Exception e) {
            e.printStackTrace();
        }
    }

    public static void readVGM(File file) {
        byte[] music = new byte[(int) file.length()];
        try {
            FileInputStream fis = new FileInputStream(file);
            fis.read(music, 0, music.length);
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }

        int checksum = 0;
        for (int i = 0; i < music.length; i++) {
            checksum += music[i];
        }
        System.out.println(music.length + " gelesen, checksum: " + checksum);

        //ident check
        if (!(music[0] == 0x56 && music[1] == 0x67 && music[2] == 0x6d && music[3] == 0x20)) {
            System.out.println("ident failed!");
            return;
        }

        //version
        int version = ((music[8] << 0) & 0x000000FF) | ((music[9] << 8) & 0x0000FF00)
                | ((music[10] << 16) & 0x00FF0000) | ((music[11] << 24) & 0xFF000000);
        System.out.println(BCDtoString(music[11]) + "." + BCDtoString(music[10]) + "." + BCDtoString(music[9]) + "." + BCDtoString(music[8]) + "   " + version);

        //total samples
        int samples = ((music[24] << 0) & 0x000000FF) | ((music[25] << 8) & 0x0000FF00)
                | ((music[26] << 16) & 0x00FF0000) | ((music[27] << 24) & 0xFF000000);
        System.out.println("samples: " + samples);

        if (true) {
            //return;
        }

        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        bos.write(music, 0, 64);

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
                //System.out.printf("PORT_0: %x %x\n", music[i + 1], music[i + 2]);
                bos.write(music, i, 3);
                i += 2;
            } else if (music[i] == (byte) 0x53) {
                //0x53 aa dd : YM2612 port 1, write value dd to register aa
                //write_data(music[i + 1], music[i + 2], PORT_1);
                //System.out.printf("PORT_1: %x %x\n", music[i + 1], music[i + 2]);
                bos.write(music, i, 3);
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
                System.out.printf("data block, type: %d, size: %d\n", data_type, size);
                datablockOffset = i + 7;
                i += 6 + size;
            } else if (music[i] == (byte) 0xE0) {
                //0xE0	dddddddd	Seek to offset dddddddd (Intel byte order) in PCM data bank of data block type 0 (YM2612).
                int offset = ((music[i + 1] << 0) & 0x000000FF) | ((music[i + 2] << 8) & 0x0000FF00)
                        | ((music[i + 3] << 16) & 0x00FF0000) | ((music[i + 4] << 24) & 0xFF000000);
                datablockIndex = offset;
                //System.out.printf("PCM data, offset: %d\n", offset);
                i += 4;
            } /* else if (music[i] == (byte) 0x93) {
                //Start Stream: 0x93 ss aa aa aa aa mm ll ll ll ll
                byte streamId = music[i + 1];
                long blockAddress = ((music[i + 2] << 0) & 0x000000FF) | ((music[i + 3] << 8) & 0x0000FF00)
                        | ((music[i + 4] << 16) & 0x00FF0000) | ((music[i + 5] << 24) & 0xFF000000);
//                  mm = Length Mode (how the Data Length is calculated)
//                  00 - ignore (just change current data position)
//                  01 - length = number of commands
//                  02 - length in msec
//                  03 - play until end of data
//                  1? - (bit 4) Reverse Mode
//                  8? - (bit 7) Loop (automatically restarts when finished)
//
//                long mode = music[i + 6];
//                long dataLength = ((music[i + 7] << 0) & 0x000000FF) | ((music[i + 8] << 8) & 0x0000FF00)
//                        | ((music[i + 9] << 16) & 0x00FF0000) | ((music[i + 10] << 24) & 0xFF000000);
                System.out.printf("start stream id: 0x%x address: %d mode: 0x%x length: %d\n", streamId, blockAddress, mode, dataLength);
                i += 10;
            }  else if (music[i] >= (byte) 0x30 && music[i] <= (byte) 0x3f) {
                //0x30..0x3F dd          : one operand, reserved for future use Note: used for dual-chip support (see below)
                System.out.printf("future use 0x%x 0x%x\n", music[i], music[i + 1]);
                i += 1;
            } */else if (music[i] == (byte) 0x66) {
                //0x66       : end of sound data
                bos.write(music, i, 1);
                break;
            } else {
                System.out.printf("unbekanntes zeichen 0x%x pos:%d\n", music[i], i);
                break;
            }


            if (waitSamples > 0) {
                //System.out.printf("wait samples: %d ys: %d\n", waitSamples, (waitSamples * 1000000) / 44100);
                //System.out.println("time passed " + (System.currentTimeMillis() - startTime) / 1000 + "s");
                //long waitNanoSec = (waitSamples * 1000000000) / 44100;
                //long waitMicrosSec = (waitSamples * 1000000) / 44100;
                //busyWaitMicros(waitMicrosSec);
            }

        }
        data = bos.toByteArray();
        System.out.printf("byte array: %d", data.length);
        System.out.printf("samples: %d ms: %d\n\n", samplesSum, (samplesSum * 1000) / 44100);
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

    static String getPortTypeName ( int portType )
    {
        switch ( portType )
        {
            case CommPortIdentifier.PORT_I2C:
                return "I2C";
            case CommPortIdentifier.PORT_PARALLEL:
                return "Parallel";
            case CommPortIdentifier.PORT_RAW:
                return "Raw";
            case CommPortIdentifier.PORT_RS485:
                return "RS485";
            case CommPortIdentifier.PORT_SERIAL:
                return "Serial";
            default:
                return "unknown type";
        }
    }
}