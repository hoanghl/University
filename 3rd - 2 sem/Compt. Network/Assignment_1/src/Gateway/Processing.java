package Gateway;

import General.Packet;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.LinkedList;

public class Processing implements Runnable {
    private LinkedList<DatagramPacket>  _queue   = new LinkedList<>();
    private Gateway                     _gateway;
    private DatagramSocket              _sender;

    // *** TESTING
    private String _thrID;
    private final Object lock = new Object();
    // *** END TESTING


    public Processing(Gateway gateway, DatagramSocket sender, String thrID) {
        this._gateway = gateway; this._sender = sender;

        this._thrID = thrID;
    }



    /**
     * This method is used by 2 threads with different goals <br>
     *     Main thread: add Datagram packet => packet = instance, flag = 1, return null <br>
     *     Processing thread: pop packet from queue => packet = null, flag = 0, return instance
     * @param packet Main thread: instance, Processing thread: null
     * @param flag Main thread: 1, Processing thread: 0
     * @return Main thread: null, Processing thread: instance
     */
    synchronized public DatagramPacket   access(DatagramPacket packet, int flag) {
        if (flag == 1) {
            // Main thread in action

            this._queue.addLast(packet);
            synchronized (lock){
                lock.notify();
            }
            return null;
        }
        else {
            // Processing thread in action

            return this._queue.removeFirst();
        }
    }


    public void run() {
        System.out.println("Thread: " + this._thrID + " starts.");


        while (true) {
            if (this._queue.isEmpty()) {
                try {
                    synchronized (lock) {
                        lock.wait();
                    }
                } catch (InterruptedException e) {
                    continue;
                }
            }
            else {
                DatagramPacket cliDPacket = access(null, 0);
                DatagramPacket resDPacket = null;

                Packet clientPacket = new Packet();
                Packet responsePacket = null;

                // extract packet from client
                clientPacket.getPacketFromClient(cliDPacket.getData());


                switch (clientPacket.get_packetID()) {
                    case Packet.CONNECT: {
                        System.out.println("CONNECT packet received");


                        if (!clientPacket.get_password().equals(Gateway.password) || this._gateway.checkDeviceID(clientPacket.get_deviceID()))
                            responsePacket = new Packet(Packet.CONNECTOK, Packet.FAILEDMSG);
                        else {
                            // deviceID not available ==> new device
                            this._gateway.addDeviceID(clientPacket.get_deviceID());     // add to deviceList

                            responsePacket = new Packet(Packet.CONNECTOK, Gateway.intvClient);
                        }
                        break;
                    }
                    case Packet.DISCONNECT: {
                        System.out.println("DISCONNECT packet received");


                        if (!this._gateway.checkDeviceID(clientPacket.get_deviceID()))
                            responsePacket = new Packet(Packet.DISCONNECTOK, Packet.FAILEDMSG);
                        else {
                            this._gateway.removeDeviceID(clientPacket.get_deviceID());

                            responsePacket = new Packet(Packet.DISCONNECTOK, 0);
                        }
                        break;
                    }
                    case Packet.DATA: {
                        System.out.println("DATA packet received");



                        if (!this._gateway.checkDeviceID(clientPacket.get_deviceID()))
                            responsePacket = new Packet(Packet.DATAOK, Packet.FAILEDMSG);
                        else {
                            // add bytes in packet to LinkedList

                            this._gateway.access_sendBytes(clientPacket.get_data(), 1);

                            // response DATAOK = interval
                            responsePacket = new Packet(Packet.DATAOK, Gateway.intvClient);
                        }
                        break;
                    }
                    default: {
                        continue;
                    }
                }

                byte[] tmp = responsePacket.toBytes();
                if (tmp == null)
                    continue;

                resDPacket = new DatagramPacket(tmp, tmp.length, cliDPacket.getAddress(), cliDPacket.getPort());
                try {
                    this._sender.send(resDPacket);
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }
}
