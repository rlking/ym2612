import gnu.io.*;

import java.io.*;

public class Main {
    public static File vgm = new File("C:\\Users\\rlking\\island.vgm");
    public static byte[] data;
    public static int sent;

    public static InputStream in;
    public static OutputStream out;

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
                    if (data == 'k') {
                        ready = true;
                    } else if (data == '\0') {
                        System.out.println(strBuff);
                        strBuff = new StringBuffer();
                        continue;
                    } else {
                        strBuff.append((char) data);
                    }
                    //ready = true;
                }
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
                while (sent != data.length) {
                    while (!ready) ;
                    ready = false;
                    System.out.println("sent data: " + (data[sent + 0] & 0xFF) + " " + (data[sent + 1] & 0xFF) + " delay: " + (System.currentTimeMillis() - time));
                    time = System.currentTimeMillis();
                    for (int i = 0; i < 64 && sent < data.length; i++, sent++) {
                        out.write(data[sent]);
                    }

                    out.flush();
                }
                System.out.println("wrote: " + data.length);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static volatile boolean ready = false;

    public static void main(String[] args) {
        data = new byte[(int) vgm.length()];

        try {
            FileInputStream fis = new FileInputStream(vgm);
            fis.read(data, 0, data.length);
        } catch (IOException ioe) {
            ioe.printStackTrace();
        }
        System.out.println(data.length + " gelesen");

        try {
            CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier("COM3");
            if (portIdentifier.isCurrentlyOwned()) {
                System.out.println("Error: Port is currently in use");
            } else {
                CommPort commPort = portIdentifier.open("vgmread", 2000);

                if (commPort instanceof SerialPort) {
                    SerialPort serialPort = (SerialPort) commPort;
                    serialPort.setSerialPortParams(1048576, SerialPort.DATABITS_8, SerialPort.STOPBITS_1, SerialPort.PARITY_NONE);

                    in = serialPort.getInputStream();
                    out = serialPort.getOutputStream();

                    serialPort.addEventListener(new SerialReader(in));
                    serialPort.notifyOnDataAvailable(true);
                    serialPort.notifyOnOverrunError(true);

                    Thread.sleep(1500); // goood lords

                    (new Thread(new SerialWriter(out))).start();
                }
            }
        } catch (
                Exception e) {
            e.printStackTrace();
        }
    }
}