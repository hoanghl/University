package Tester;

import Gateway.Gateway;
import General.General_Resources;
import General.Packet;

import javax.xml.crypto.Data;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Random;

public class UDPClient {
    public static void main(String[] args) throws Exception{
        DatagramSocket gateway = new DatagramSocket();

        Packet connectPacket = new Packet("99092", Gateway.password);

        byte[] tmp1 = connectPacket.toBytes();
        DatagramPacket pack1 = new DatagramPacket(tmp1, tmp1.length, InetAddress.getByName("localhost"), General_Resources.GATEWAY_PORT);
        gateway.send(pack1);

        byte[] recv = new byte[1024];

        DatagramPacket packet2 = new DatagramPacket(recv, recv.length);
        gateway.receive(packet2);

        Packet rcvPack = new Packet();
        rcvPack.getPacketFromClient(recv);
        System.out.println(rcvPack.get_packetID() + " received.");

        Random random = new Random(90);
        while (true) {
            Thread.sleep(2000);

            Packet dataPacket = new Packet(Packet.DATA, "99092");
            Byte[] dat = new Byte[random.nextInt(20)];
            System.out.println("Client is about to send: ");
            for (int i = 0; i < dat.length; i++) {
                dat[i] = (byte) random.nextInt(40);
                System.out.println(dat[i]);
            }
            System.out.println("End");

            dataPacket.addBytes(dat);
            byte[] tmp = dataPacket.toBytes();
            gateway.send(new DatagramPacket(tmp,tmp.length, InetAddress.getByName("localhost"), General_Resources.GATEWAY_PORT));

            Packet dataOKPacket = new Packet();
            DatagramPacket rcvPacket = new DatagramPacket(recv, 1024);
            gateway.receive(rcvPacket);
            dataOKPacket.getPacketFromClient(rcvPacket.getData());

            System.out.println("Received: " + dataOKPacket.get_packetID());
        }
    }

}
