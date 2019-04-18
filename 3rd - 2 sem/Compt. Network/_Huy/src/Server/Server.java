package Server;

import Gateway.Gateway;
import General.General_Resources;
import General.Packet;

import java.awt.BorderLayout;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.net.*;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Arrays;
import java.util.LinkedList;

import javax.swing.JFrame;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;

public class Server {

	static int x = 0;
	
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
    	JFrame window = new JFrame();
		window.setTitle("Sensor Graph GUI");
		window.setSize(600, 400);
		window.setLayout(new BorderLayout());
		window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		XYSeries series = new XYSeries("Sensor Readings");
		XYSeriesCollection dataset = new XYSeriesCollection(series);
		JFreeChart chart = ChartFactory.createXYLineChart("Sensor Readings", "Time (seconds)", "Reading", dataset);
		window.add(new ChartPanel(chart), BorderLayout.CENTER);
		window.setVisible(true);


        ServerSocket server = new ServerSocket(General_Resources.SERVER_PORT);
        Server tester = new Server();

        DateTimeFormatter date = DateTimeFormatter.ofPattern("yyyy/MM/dd HH:mm:ss");
    	PrintWriter writer = new PrintWriter("log.txt");
        while (true) {
        	int avg = 0;
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
                        int tmp = Server.getIntFromBytes(Arrays.copyOfRange(deviceID, 0,4 ));
                        System.out.println("Len in server: " + tmp);
                        //LinkedList<Byte> readBytes = new LinkedList<>();
                        for (int i = 0; i < tmp; i++) {
                            //readBytes.addLast(inputStream.readByte());
                        	int a = inputStream.readByte();
                            System.out.println("Read: " + a);
                            avg += a;
                        }
                        avg = avg/tmp;
                        series.add(x+=5,avg);
                        window.repaint();
                        PrintWriter writer2 = new PrintWriter(new FileOutputStream(new File("log.txt"),true ));
                        writer2.println(avg);
                        writer2.close();
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
