package Server;

import java.io.DataInputStream;
import java.io.DataOutput;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.LinkedList;

public class Week_3_Server {
    public static void main(String[] args) {
        try {
            ServerSocket server = new ServerSocket(43321);

            while (true){
                System.out.println("Server is waiting ...");

                Socket client = server.accept();
                DataInputStream  dataInputStream  = new DataInputStream(client.getInputStream());
                DataOutputStream dataOutputStream = new DataOutputStream(client.getOutputStream());

                int n_bytes = dataInputStream.readByte();
                LinkedList<Byte> _bytes = new LinkedList<>();
                for (int i = 0; i < n_bytes; i++)
                    _bytes.add(dataInputStream.readByte());

                System.out.println("Message from client " + client.getInetAddress().toString() + ": ");
                for (Byte i : _bytes)
                    System.out.print((char)i.byteValue());

                System.out.println("Sending response code to " + client.getInetAddress().toString());
                dataOutputStream.writeBytes("OK");
                client.close();
                System.out.println(" *** Server has closed the connection. *** ");
            }
        } catch (IOException e){
            e.printStackTrace();
        }
    }
}
