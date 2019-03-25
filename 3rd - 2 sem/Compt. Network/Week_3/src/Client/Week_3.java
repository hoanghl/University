package Client;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.nio.charset.Charset;

public class Week_3 {
    public static void main(String[] args) {
        try {
            InetAddress serAddr = InetAddress.getLocalHost();
            Socket socket = new Socket(serAddr, Integer.parseInt(args[1]));
            System.out.println("Connection successfully.");

            DataOutputStream dataOutputStream = new DataOutputStream(socket.getOutputStream());
            DataInputStream  dataInputStream  = new DataInputStream(socket.getInputStream());

            String _msg = "Client sends message to server.";
            dataOutputStream.write(_msg.length());
            dataOutputStream.write(_msg.getBytes(Charset.defaultCharset()));

            Byte[] _rspMsg = new Byte[2];
            for (int i = 0; i < 2; i++)
                _rspMsg[i] = dataInputStream.readByte();
            System.out.print("Server response code: " + (char)_rspMsg[0].intValue() + (char)_rspMsg[1].intValue());

            socket.close();
        } catch (UnknownHostException e){
            e.printStackTrace();
        } catch (IOException e){
            e.printStackTrace();
        }
    }
}
