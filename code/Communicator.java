import java.util.Arrays;
import java.util.Objects;
import java.util.Properties;
import java.io.IOException;
import java.io.UncheckedIOException;

import java.awt.Image;
import java.io.ByteArrayInputStream;
import javax.imageio.ImageIO;
import javax.imageio.stream.ImageInputStream;
import javax.imageio.stream.MemoryCacheImageInputStream;
import java.util.function.Consumer;

public class Communicator {
	/****************************  CONFIGURATION   ****************************/
	private static final Properties secret = new Properties();
	private static final String RPI_HOST;
	private static final String PC_HOST;
	private static final int CMD_PORT;
	private static final int CAM_PORT;
	private static final UdpSocket transportCmd;
	private static final UdpSocket transportCam;
	static { try {
		secret.load(new java.io.FileReader("secret.py"));
		String rpiHost = secret.getProperty("RPI_HOST");
		RPI_HOST = rpiHost.substring(1, rpiHost.length() - 1);
		String pcHost = secret.getProperty("PC_HOST");
		PC_HOST = pcHost.substring(1, pcHost.length() - 1);
		CMD_PORT = Integer.parseInt(secret.getProperty("CMD_PORT"));
		CAM_PORT = Integer.parseInt(secret.getProperty("CAM_PORT"));
		
		transportCmd = new UdpSocket(RPI_HOST, CMD_PORT, false);
		transportCam = new UdpSocket(PC_HOST, CAM_PORT, true);
	} catch (IOException e) { throw new UncheckedIOException(e); } }
	
	/**************************** KEY TRANSMITTER  ****************************/
	public static final int KEY_W =  0; // step forward
	public static final int KEY_A =  1; // step backward
	public static final int KEY_S =  2; // pivot left
	public static final int KEY_D =  3; // pivot right
	public static final int KEY_R =  4; // full standing
	public static final int KEY_F =  5; // full crouching 
	public static final int KEY_T =  6; // claw close
	public static final int KEY_G =  7; // claw open
	public static final int KEY_1 =  8; // claw arm top pos
	public static final int KEY_2 =  9; // claw arm fwd pos
	public static final int KEY_3 = 10; // claw arm mid pos
	public static final int KEY_4 = 11; // claw arm right pos
	public static final int KEY_UNKNOWN = 12;
	public static final byte[]  chars = {
		'w', 'a', 's', 'd', 'r', 'f', 't', 'g', '1', '2', '3', '4', '?'
	};
	private final boolean[] keys;
	private boolean runningCmd = true;
	public static final int CMD_PERIOD = 1000 / 20;
	
	public void cmdLoop() { try {
		while (runningCmd) {
			final long nextStart = System.currentTimeMillis() + CMD_PERIOD;
			for (int i = 0; i < KEY_UNKNOWN; ++i) if (keys[i]) {
				transportCmd.send(Arrays.copyOfRange(chars, i, i+1));
				keys[i] = false;
				break;
			}
			// don't send anything if no keys pressed
			final long throttleDelay = nextStart - System.currentTimeMillis();
			if (throttleDelay > 0) {
				try { Thread.sleep(throttleDelay); }
				catch (InterruptedException e) { }
			}
		}
		transportCmd.send(new byte[] { (byte) ' ' });
	} catch (IOException e) { throw new UncheckedIOException(e); } }
	
	/**************************** IMAGE RECEIVER   ****************************/
	private final Consumer<Image> display;
	private boolean runningCam = true;
	public void camLoop() { try {
		while (runningCam) {
			final byte[] jpeg = transportCam.recvBig();
			if (jpeg == null) {
				System.err.println("Warning: Failed to receive camera image!");
				continue;
			}
			final ImageInputStream imgIn = new MemoryCacheImageInputStream(new ByteArrayInputStream(jpeg));
			final Image image = ImageIO.read(imgIn);
			if (image == null) {
				System.err.println("Warning: Failed to decode received frame!");
				imgIn.close();
				continue;
			}
			display.accept(image);
		}
	} catch (IOException e) { throw new UncheckedIOException(e); } }
	
	/**************************** CONTROL METHODS  ****************************/
	public Communicator(final boolean[] keys, final Consumer<Image> display) {
		this.keys = Objects.requireNonNull(keys);
		this.display = Objects.requireNonNull(display);
	}
	
	public void start() {
		final Thread cmdThread = new Thread(this::cmdLoop);
		cmdThread.setName("KeyboardCommander");
		cmdThread.start();
		final Thread imgThread = new Thread(this::camLoop);
		imgThread.setName("ImageAccepter");
		imgThread.start();
	}
	
	public void shutdown() {
		runningCmd = false;
		runningCam = false;
	}
}