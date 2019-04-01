package Gateway;

import General.General_Resources;
import General.Packet;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;


public class ContactServer implements Runnable {
    private final Object    lock    = new Object();
    private Gateway   _gateway;

    public ContactServer(Gateway gateway) {
        this._gateway = gateway;
    }


    public void wake_up() {
        this.lock.notify();
    }
    /**
     * Init connection to Server
     */
    private void     connServer() {
        try {
            Socket gateway = new Socket("localhost", General_Resources.SERVER_PORT);
            DataOutputStream outputStream = new DataOutputStream(gateway.getOutputStream());

            // send CONNECT packet first time
            Packet connPacket = new Packet(this._gateway.get_deviceID(), Gateway.password);
            outputStream.write(connPacket.toBytes());

            Packet connOKPacket = new Packet();
            // constantly resend CONNECT packet until replying correctly
            while (connOKPacket.getPacketFromServer(gateway) == 1 || connOKPacket.getRepliedData() == Packet.FAILEDMSG)
                outputStream.write(connPacket.toBytes());

            // analyse CONNECTOK packet
            Gateway.intvServer = connOKPacket.getRepliedData();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void run() {
        connServer();

        while (true) {
            try {
                Thread.sleep(Gateway.intvServer);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            Packet dataPacket = new Packet(Packet.DATA, this._gateway.get_deviceID());
            dataPacket.addBytes(this._gateway.access_sendBytes(null, 0));


            try {
                Socket sender = new Socket("localhost", General_Resources.SERVER_PORT);
                DataOutputStream outputStream = new DataOutputStream(sender.getOutputStream());

                // send DATA packet to server
                outputStream.write(dataPacket.toBytes());

                // wait for DATAOK
                Packet dataOKPacket = new Packet();
                while (dataOKPacket.getPacketFromServer(sender) == 1 || dataOKPacket.getRepliedData() == 0xFFFF)
                    outputStream.write(dataPacket.toBytes());

                Gateway.intvServer = dataOKPacket.getRepliedData();

                sender.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
    }
}
