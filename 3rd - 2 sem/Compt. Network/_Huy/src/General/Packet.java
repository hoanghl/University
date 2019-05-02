package General;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.DatagramPacket;
import java.net.Socket;
import java.util.Arrays;
import java.util.LinkedList;

public class Packet {
    /**
     * This class is specific for Gateway.Gateway only.
     */
    public static final byte CONNECT       = 1;
    public static final byte DISCONNECT    = 2;
    public static final byte DATA          = 3;
    public static final byte CONNECTOK     = 4;
    public static final byte DISCONNECTOK  = 5;
    public static final byte DATAOK        = 6;

    public static final int  FAILEDMSG     = 0xFFFF;


    private byte                _packetID      = 0;
    private String              _deviceID      = null;
    private String              _password      = null;

    public int get_data_length() {
        return _data_length;
    }

    private int                 _data_length   = 0;
    private LinkedList<Byte>    _data          = null;

    public Packet() {}

    /**
     * Used for creating CONNECTOK, DISCONNECTOK, DATAOK
     *
     *  @param packetID packetID of the packet
     */
    public Packet(byte packetID, int value) {
        this._packetID = packetID;

        if (value == Packet.FAILEDMSG) {
            this._data_length = 2;
            this._data = new LinkedList<>();
            this._data.addLast((byte)0xFF);
            this._data.addLast((byte)0xFF);
        }
        else {
            this._data_length = 4;
            this._data = new LinkedList<>();

            for (Byte b : this.getBytesFromInt(value))
                this._data.addLast(b);
        }
    }

    /**
     * Used for creating DATA, DISCONNECT
     */
    public Packet(byte packetID, String deviceID) {
        this._packetID = packetID; this._deviceID = deviceID;
    }

    /**
     * Used for creating CONNECT
     */
    public Packet(String deviceID, String password) {
        this._packetID = Packet.CONNECT;
        this._deviceID = deviceID; this._password = password;
    }

    /*
       Getter
     */
    public byte     get_packetID() {
        return _packetID;
    }

    public String   get_deviceID() {
        return _deviceID;
    }

    public String   get_password() {
        return _password;
    }

    public byte[]   get_data() {
        byte[] tmp = new byte[this._data.size()];
        int i = 0;
        for (Byte b : this._data.toArray(new Byte[this._data.size()]))
            tmp[i++] = b;

        return tmp;
    }

    /**
     * Used for creating DATA to send to Server <br>
     * Embed bytes to packet's payload
     * @param bytes - bytes to add
     */
    public void     addBytes(Byte[] bytes) {
        if (this._data == null)
            this._data = new LinkedList<>();

        this._data_length += bytes.length;
        for (byte b : bytes)
            this._data.addLast(b);
    }

    /**
     * covert all fields of Packet to byte for sending
     * @return byte array is convert successes
     *         null
     */
    public byte[]   toBytes() {
        //byte[] bytes = null;
        LinkedList<Byte> extratingBytes = new LinkedList<>();

        extratingBytes.addLast(this._packetID);


        if (this._password != null)
            for (char c : this._password.toCharArray())
                extratingBytes.addLast((byte) c);


        if (this._deviceID != null)
            for (char c : this._deviceID.toCharArray())
                extratingBytes.addLast((byte) c);



        for (byte b : this.getBytesFromInt(this._data_length))
            extratingBytes.addLast(b);

        if (this._data != null)
            for (Byte b : this._data)
                extratingBytes.addLast(b);

        int i = 0;
        byte[] tmp = new byte[extratingBytes.toArray().length];

        for ( Byte b : extratingBytes.toArray(new Byte[extratingBytes.size()]))
            tmp[i++] = b;


        return  tmp;
    }


    /*
     * The following methods are used by Gateway to communicate with Server
     */

    /**
     * Gateway <-- Server <br>
     * Gateway uses this method to receive packet from Server
     * @param client A client
     * @return An integer number indicates whether the error occurs
     *          0: no error
     *          1: some byte(s) not correct
     */
    public int      getPacketFromServer(Socket client) {
        try {
            DataInputStream inputStream = new DataInputStream(client.getInputStream());

            // receive packetID
            Byte firstByte = inputStream.readByte();
            if (firstByte < Packet.CONNECT || firstByte > Packet.DATAOK)
                return 1;
            else
                this._packetID = firstByte;

            // receive next 4 bytes
            byte[] bytes = new byte[4];
            for (int i = 0; i < 4; i++)
                bytes[i] = inputStream.readByte();


            int data_length = getIntFromBytes(bytes);
            if (data_length > 100)
                return 1;
            else
                this._data_length = data_length;

            //receive payload
            if (this._data == null)
                this._data = new LinkedList<>();
            else
                this._data.clear();                             // this line ensures that in case receiving bytes in the previous receiving time
                                                                // are not correct, this._data doesn't recreate new instance.

            for (int i = 0; i < this._data_length; i++)
                this._data.add(inputStream.readByte());


        } catch (IOException e) {
            e.printStackTrace();
        }

        return 0;
    }

    /**
     * Gateway <-- Server <br>
     * Return the value the server embeds in the replying packet. This value may be time interval or 0xFFFF <br>
     * Only apply for CONNECTOK, DATAOK packet
     * @return positive value: 0xFFFF or time interval
     *         -1 if incorrect byte received or the packet instance is not initialized
     */
    public int      getRepliedData() {
        if (this._data == null)
            return -1;

        byte[] bytes = new byte[this._data.size()];
        int i = 0;
        for (Byte b : (this._data.toArray(new Byte[this._data.size()])))
            bytes[i++] = b;

        int interval = getIntFromBytes(bytes);

        if (interval > General_Resources.MAX_INTERVAL || interval <= 0)
            return -1;                                                      // incorrect byte receiving leads to unreasonable converting result
        else
            return interval;
    }



    /*
     * The following methods are used by Gateway to communicate with Client
     */

    /**
     * Gateway <-- Client <br>
     * Gateway uses this to export UDP packet from Client.
     * The stuffs extracted from datagram, if correct, are put to fields of this object <br>
     * The packet Gateway may receive: CONNECT, DISCONNECT, DATA
     * @param datagram Bytes of UDP datagram Gateway receives
     * @return 0: if extraction not error
     *         1: error byte occurs
     */
    public int      getPacketFromClient(byte[] datagram) {


        if (datagram[0] < Packet.CONNECT || datagram[0] > Packet.DATAOK)
            return 1;

        this._packetID = datagram[0];
        // extract password
        byte pos = 1;
        if (datagram[0] == Packet.CONNECT) {
            this._password = new String(datagram, 1, 10);
            pos += 10;
        }

        // extract deviceID, data_length and data field
        this._deviceID = new String(datagram, pos, 5);
        pos += 5;

        this._data_length = this.getIntFromBytes(Arrays.copyOfRange(datagram, pos, (pos += 4)));

        this._data = new LinkedList<>();
        for (int i = 0; i < this._data_length; pos++, i++)
            this._data.addLast(datagram[pos]);

        return 0;
    }



    /*
       Private methods
     */
    private int     getIntFromBytes(byte[] bytes) {
        int tmp = 0;
        for (int i = bytes.length - 1; i >= 0; i--)
            tmp += bytes[i] << (4 * (bytes.length - i - 1));
        return tmp;
    }
    private Byte[]  getBytesFromInt(int n) {
        Byte[] bytes = new Byte[4];
        bytes[0] = (byte)(n & 0xFF000000);
        bytes[1] = (byte)(n & 0x00FF0000);
        bytes[2] = (byte)(n & 0x0000FF00);
        bytes[3] = (byte)(n & 0x000000FF);

        return bytes;
    }
}