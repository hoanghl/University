package Tester;

import Gateway.Gateway;
import General.General_Resources;
import General.Packet;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.util.Arrays;

public class Main {
    public static void main(String[] args) throws Exception{
        /*
                1. Test Processing module
         */

        // ** CONNECT ** packet
        Packet packet = new Packet("09908", Gateway.password);
        DatagramSocket clientSock = new DatagramSocket();


        byte[] sendData = packet.toBytes();


        byte[] receiveData = new byte[20];

        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, InetAddress.getByName("localhost"), General_Resources.GATEWAY_PORT);
        clientSock.send(sendPacket);


        DatagramPacket recvPacket = new DatagramPacket(receiveData, receiveData.length);
        clientSock.receive(recvPacket);


        // ** DISCONNECT ** packet

        Packet packet2 = new Packet(Packet.DISCONNECT, "09908");
        sendData = packet2.toBytes();
        sendPacket.setData(sendData);
        sendPacket.setLength(sendData.length);

        clientSock.send(sendPacket);

        Arrays.fill(receiveData, (byte)0);

        clientSock.receive(recvPacket);
        for (byte b : receiveData)
            System.out.println("Received DATAOK: " + b);

        // ** DATA ** packet
        Packet packet1 = new Packet(Packet.DATA, "09908");
        Byte[] data = {1, 0, 0, 0, 21, 4, 0, 6, 4 ,2, 8, 1, 0, 0, 1};
        packet1.addBytes(data);

        Arrays.fill(receiveData, (byte)0);

        sendData = packet1.toBytes();
        sendPacket.setData(sendData);
        sendPacket.setLength(sendData.length);


        clientSock.send(sendPacket);
        clientSock.receive(recvPacket);
        for (byte b : receiveData)
            System.out.println("Received DATAOK: " + b);
    }
}
