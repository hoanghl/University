package Tester;

import Gateway.Gateway;
import General.General_Resources;
import General.Packet;

import java.io.DataInput;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Arrays;
import java.util.LinkedList;

public class Main {

    private LinkedList<String>      _deviceList = new LinkedList<>();
    public boolean isIn(String deviceID) {
        return  this._deviceList.lastIndexOf(deviceID) != -1;
    }
    public void     addDevice(String deviceID) {
        this._deviceList.addLast(deviceID);
    }
    public boolean  removeDevice(String deviceID) {
        return this._deviceList.remove(deviceID);
    }

    public static int     getIntFromBytes(byte[] bytes) {
        int tmp = 0;
        for (int i = bytes.length - 1; i >= 0; i--)
            tmp += bytes[i] << (4 * (bytes.length - i - 1));
        return tmp;
    }

    public static void main(String[] args) throws Exception{
        /*
                1. Test Processing module
         */



        ServerSocket server = new ServerSocket(General_Resources.SERVER_PORT);
        Main tester = new Main();

        DateTimeFormatter date = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");

        while (true) {
            System.out.println("Server is on ...");

            Socket client = server.accept();
            System.out.println("Server received a connection at: " + date.format(LocalDateTime.now()));

            DataInputStream inputStream = new DataInputStream(client.getInputStream());
            DataOutputStream outputStream = new DataOutputStream(client.getOutputStream());

            Packet response = null;
            switch (inputStream.readByte()) {
                case Packet.CONNECT: {
                    System.out.println("CONNECT received.");

                    // read password
                    byte[] password = new byte[10];
                    inputStream.read(password, 0, 10);

                    // read deviceID
                    byte[] deviceid = new byte[5];
                    inputStream.read(deviceid, 0, 5);
                    String deviceID = new String(password);


                    if (!Gateway.password.equals(new String(password)) || tester.isIn(deviceID)) {
                        response = new Packet(Packet.CONNECTOK, Packet.FAILEDMSG);
                    }
                    else {
                        response = new Packet(Packet.CONNECTOK, 5);
                        tester.addDevice(deviceID);
                    }

                    break;
                }
                case Packet.DISCONNECT: {
                    System.out.println("DISCONNECT received.");


                    byte[] deviceID = new byte[5];
                    inputStream.read(deviceID, 0, 5);
                    if (!tester.removeDevice(new String(deviceID)))
                        response = new Packet(Packet.DISCONNECTOK, Packet.FAILEDMSG);
                    else
                        response = new Packet(Packet.DISCONNECTOK, 0);

                    break;
                }
                case Packet.DATA: {
                    System.out.println("DATA received.");


                    byte[] deviceID = new byte[5];
                    inputStream.read(deviceID, 0, 5);
                    if (tester.isIn(new String(deviceID)))
                        response = new Packet(Packet.DATAOK, Packet.FAILEDMSG);
                    else {
                        Arrays.fill(deviceID, (byte)0);
                        inputStream.read(deviceID, 0, 4);

//                        for (byte b : Arrays.copyOfRange(deviceID, 0,3 )){
//                            System.out.println("Read byte : " + b);
//                        }

                        int tmp = Main.getIntFromBytes(Arrays.copyOfRange(deviceID, 0,4 ));
                        System.out.println("Len in server: " + tmp);
                        //LinkedList<Byte> readBytes = new LinkedList<>();
                        for (int i = 0; i < tmp; i++) {
                            //readBytes.addLast(inputStream.readByte());
                            System.out.println("Read: " + inputStream.readByte());
                        }
                        response = new Packet(Packet.DATAOK, 5);
                    }

                }
            }

            try {
                outputStream.write(response.toBytes());
            } catch (NullPointerException e) {
                e.printStackTrace();
            }

            client.close();
        }
    }
}
