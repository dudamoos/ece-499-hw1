import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import javax.swing.JFrame;
import javax.swing.JButton;

import java.util.Arrays;
import java.util.Properties;
import java.io.IOException;

public class InputClient extends JFrame implements WindowListener, KeyListener, Runnable {
	/**************************** KEY POLLER       ****************************/
	public static final int KEY_I = 0; // Forward
	public static final int KEY_J = 1; // Pivot Left
	public static final int KEY_K = 2; // Backward
	public static final int KEY_L = 3; // Pivot Right
	public static final int KEY_STOP = 4; // Stop
	public static final int KEY_UNKNOWN = 5;
	public static final byte[] chars = { 'i', 'j', 'k', 'l', ' ', '?' };
	final boolean[] keys = new boolean[6]; // polling array
	{ keys[KEY_STOP] = true; }
	public static int keyIndex(KeyEvent e) {
		switch (e.getKeyCode()) {
			case KeyEvent.VK_I: return KEY_I;
			case KeyEvent.VK_J: return KEY_J;
			case KeyEvent.VK_K: return KEY_K;
			case KeyEvent.VK_L: return KEY_L;
			default: return KEY_UNKNOWN;
		}
	}
	
	// interesting events for polling
	@Override public void windowClosing(WindowEvent e) { running = false; }
	@Override public void keyPressed(KeyEvent e) { keys[keyIndex(e)] = true; }
	@Override public void keyReleased(KeyEvent e) { keys[keyIndex(e)] = false; }
	// others
	@Override public void keyTyped(KeyEvent arg0) { }
	@Override public void windowActivated(WindowEvent arg0) { }
	@Override public void windowDeactivated(WindowEvent arg0) { }
	@Override public void windowDeiconified(WindowEvent arg0) { }
	@Override public void windowIconified(WindowEvent arg0) { }
	@Override public void windowOpened(WindowEvent e) { }
	@Override public void windowClosed(WindowEvent e) { }
	
	/**************************** KEY TRANSMITTER  ****************************/
	private static final Properties secret = new Properties();
	private static final String RPI_HOST;
	private static final int CMD_PORT;
	private static final UdpSocket transport;
	static { try {
		secret.load(new java.io.FileReader("secret.py"));
		String rpiHost = secret.getProperty("RPI_HOST");
		RPI_HOST = rpiHost.substring(1, rpiHost.length() - 1);
		CMD_PORT = Integer.parseInt(secret.getProperty("CMD_PORT"));
		transport = new UdpSocket(RPI_HOST, CMD_PORT);
	} catch (IOException e) { throw new java.io.UncheckedIOException(e); } }
	boolean running = true;
	public static final int PERIOD = 1000 / 1;
	
	@Override public void run() {
		try {
			while (running) {
				long curTime = System.currentTimeMillis();
				long nextStart = curTime + PERIOD;
				for (int i = 0; i < KEY_UNKNOWN; i++) if (keys[i]) {
					transport.send(Arrays.copyOfRange(chars, i, i+1));
					System.out.println("Sent command: '" + ((char) chars[i]) + "' @ " + curTime);
					break;
				}
				long throttleDelay = nextStart - System.currentTimeMillis();
				if (throttleDelay > 0) {
					try { Thread.sleep(throttleDelay); }
					catch (InterruptedException e) { }
				}
			}
			transport.send(Arrays.copyOfRange(chars, KEY_STOP, KEY_UNKNOWN));
		} catch (IOException e) { throw new java.io.UncheckedIOException(e); }
	}
	
	/**************************** CLIENT WINDOW    ****************************/
	public InputClient() {
		addKeyListener(this);
		addWindowListener(this);
		JButton button = new JButton("Key Poller");
		button.addKeyListener(this);
		add(button);
		
		pack();
		setVisible(true);
		
		Thread netThread = new Thread(this);
		netThread.setName("KeyboardCommander");
		netThread.start();
	}
	
	/**************************** KEY TRANSMITTER  ****************************/
	public static void main(String[] args) {
		InputClient client = new InputClient();
		client.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}