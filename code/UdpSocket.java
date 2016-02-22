import java.net.DatagramSocket;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.SocketAddress;
import java.net.SocketException;

import java.util.Arrays;
import java.util.Objects;
import java.util.stream.IntStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import static java.nio.ByteOrder.LITTLE_ENDIAN;

public class UdpSocket {
	// Should agree with udp.py
	public static final int MAX_CHUNK_SIZE = 32768;
	
	private final SocketAddress addr;
	private final DatagramSocket sock;
	
	public UdpSocket(String host, int port                   ) throws SocketException { this(new InetSocketAddress(host, port)); }
	public UdpSocket(String host, int port, boolean receiving) throws SocketException { this(new InetSocketAddress(host, port), receiving); }
	public UdpSocket(InetAddress host, int port                   ) throws SocketException { this(new InetSocketAddress(host, port)); }
	public UdpSocket(InetAddress host, int port, boolean receiving) throws SocketException { this(new InetSocketAddress(host, port), receiving); }
	public UdpSocket(SocketAddress addr                   ) throws SocketException { this(addr, false); }
	public UdpSocket(SocketAddress addr, boolean receiving) throws SocketException {
		this.addr = Objects.requireNonNull(addr, "UdpSocket requires a send/receive address");
		this.sock = receiving ? new DatagramSocket(addr) : new DatagramSocket();
		//if (!receiving) sock.connect(addr);
	}
	// may not provide as strong a guarantee as python __del__ with except KeyboardInterrupt
	protected void finalize() { if (!sock.isClosed()) sock.close(); }
	
	public void setTimeout(int timeout) throws SocketException { sock.setSoTimeout(timeout); }
	
	public void send(byte[] data) throws IOException {
		DatagramPacket packet = new DatagramPacket(Arrays.copyOf(data, data.length + 1), data.length + 1, addr);
		// No ByteStream, so fake it with an IntStream
		packet.getData()[data.length] = (byte) ~IntStream.range(0, data.length).map(i -> data[i]).sum();
		// Assuming this sends the entire packet, because it doesn't provide a way to resume sending
		sock.send(packet);
	}
	
	// These should probably use Exceptions instead of returning false, but I don't want to deal with that right now
	public boolean recv(byte[] data) throws IOException { return recv(data, true); }
	public boolean recv(byte[] data, boolean requireFull) throws IOException {
		byte[] buf = new byte[data.length + 1];
		DatagramPacket packet = new DatagramPacket(buf, buf.length);
		sock.receive(packet);
		int last = packet.getOffset() + packet.getLength();
		if (requireFull) while(last < buf.length) {
			int remaining = buf.length - last;
			packet.setData(buf, last, remaining);
			sock.receive(packet);
			last = packet.getOffset() + packet.getLength();
		}
		byte cksum = (byte) IntStream.range(0, last - 1).map(i -> buf[i]).sum();
		if (((buf[last - 1] + cksum) & 0xFF) != 0xFF)
			return false; // corrupted data
		System.arraycopy(buf, 0, data, 0, last - 1);
		return true;
	}
	
	public void sendBig(byte[] data) throws IOException {
		Objects.requireNonNull(data);
		send(ByteBuffer.allocate(4).order(LITTLE_ENDIAN).putInt(0, data.length).array());
		int totalSent = 0;
		while (totalSent < data.length) {
			int chunkLen = Math.min(data.length - totalSent, MAX_CHUNK_SIZE);
			send(Arrays.copyOfRange(data, totalSent, totalSent + chunkLen));
			totalSent += chunkLen;
		}
	}
	
	public byte[] recvBig() throws IOException {
		byte[] buf = new byte[4];
		if (!recv(buf, true)) return null; // corrupted data
		int length = ByteBuffer.wrap(buf).order(LITTLE_ENDIAN).getInt(0);
		int received = 0;
		byte[] packet = new byte[length];
		while (received < length) {
			buf = new byte[Math.min(length - received, MAX_CHUNK_SIZE)];
			if (!recv(buf, true)) return null; // corrupted data
			System.arraycopy(buf, 0, packet, received, buf.length);
			received += buf.length;
		}
		return packet;
	}
}