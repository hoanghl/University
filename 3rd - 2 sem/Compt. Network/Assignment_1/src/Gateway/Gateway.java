package Gateway;

import General.General_Resources;
import General.Packet;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.Socket;
import java.util.LinkedList;
import java.util.Random;

public class Gateway {
    public static int               intvClient  = 2;
    public static int               intvServer  = 0;
    public static String            password    = "congsanvkl";


    // this LinkedList stores bytes sent by clients
    // only modified by 2 Processing threads
    private LinkedList<Byte>    _sendBytes   = new LinkedList<>();
    private LinkedList<String>  _deviceList  = new LinkedList<>();

    private String              _deviceID    = null;

    public  Gateway() {
        this._deviceID = this.genDeviceID();
    }

    public void                 resetDeviceID() {
        this._deviceID = this.genDeviceID();
    }
    public String               get_deviceID() {
        return this._deviceID;
    }

    /**
     * Check whether deviceID is signed up before
     * @param deviceID: deviceID of the device
     * @return true - there exists deviceID in list
     *         false - otherwise
     */
    public  boolean             checkDeviceID(String deviceID) {
        return this._deviceList.lastIndexOf(deviceID) != -1;
    }

    synchronized public void    addDeviceID(String deviceID) {
        this._deviceList.addLast(deviceID);
    }

    synchronized public void    removeDeviceID(String _deviceID) {
        this._deviceList.remove(_deviceID);
    }


    /**
     * The method is used by Processing thread and ContactServer thread <br>
     *     Processing threads add bytes extracted <br>
     *     ContactServer thread takes all byte in LinkedList <br>
     * @param added_bytes - Processing thread: instance, ContactServer: null
     * @param flag - Processing thread: 1, ContactServer: 0
     * @return Processing thread: null, ContactServer: instance
     */
    synchronized public Byte[]  access_sendBytes(byte[] added_bytes, int flag) {
        if (flag == 1) {
            // Processing thread in action

            for (byte b : added_bytes)
                this._sendBytes.addLast(b);

            return null;
        }
        else {
            // ContactServer thread in action

            Byte[] tmp = this._sendBytes.toArray(new Byte[this._sendBytes.size()]);
            this._sendBytes.clear();
            return tmp;
        }
    }

    private String    genDeviceID() {
        StringBuilder tmp = new StringBuilder();

        Random r = new Random();
        for (int i = 0; i < 5; i++)
            tmp.append(r.nextInt(10));

        return tmp.toString();
    }




    public static void main(String[] args) {
        // Main thread: receive UDP packet from client and forward packet's bytes to 2 Processing threads
        try {
            byte[] rcvBuffer = new byte[General_Resources.MAX_BUFF];
            DatagramSocket receiver = new DatagramSocket(General_Resources.GATEWAY_PORT);
            Gateway      gateway = new Gateway();



            byte[] dump_bytes = new byte[10];
            Random random = new Random();
            for (int i = 0; i < 10; i++)
                dump_bytes[i] = (byte)(random.nextInt(100));

            gateway.access_sendBytes(dump_bytes, 1);


            // create necessary threads: 1 ServerContactThread, 2 Processing
            ContactServer serverContact = new ContactServer(gateway);
            Processing    processing1   = new Processing(gateway, receiver, "T1");
            Processing    processing2   = new Processing(gateway, receiver, "T2");
            new Thread(serverContact).start();
            new Thread(processing1).start();
            new Thread(processing2).start();

            // Gateway class plays a role as a manager assigning UDP datagram to Process thread 1 and 2
            int order = 0;              // this variable always interchange between 1 and 0: 1: assign UDP datagram for Process thread 1,
                                        // otherwise 2
            while (true) {
                DatagramPacket packet = new DatagramPacket(rcvBuffer, General_Resources.MAX_BUFF);
                receiver.receive(packet);

                if (order == 0) {
                    order++;
                    processing1.access(packet, 1);
                }
                else {
                    order--;
                    processing1.access(packet, 1);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
