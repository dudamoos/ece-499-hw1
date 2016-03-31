import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.WindowEvent;
import java.awt.event.WindowListener;
import java.util.function.Consumer;

import java.awt.Canvas;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Graphics;
import java.awt.Frame;

public class InputClient extends Canvas implements WindowListener, KeyListener, Consumer<Image> {
	/**************************** KEY POLLER       ****************************/
	private final boolean[] keys = new boolean[Communicator.KEY_UNKNOWN + 1]; // polling array
	
	// interesting events for polling
	@Override public void keyPressed(KeyEvent e) { keys[keyIndex(e)] = true; }
	@Override public void keyReleased(KeyEvent e) { keys[keyIndex(e)] = false; }
	@Override public void keyTyped(KeyEvent arg0) { }
	public static int keyIndex(KeyEvent e) {
		switch (e.getKeyCode()) {
			case KeyEvent.VK_I: return Communicator.KEY_I;
			case KeyEvent.VK_J: return Communicator.KEY_J;
			case KeyEvent.VK_K: return Communicator.KEY_K;
			case KeyEvent.VK_L: return Communicator.KEY_L;
			case KeyEvent.VK_E: return Communicator.KEY_E;
			case KeyEvent.VK_D: return Communicator.KEY_D;
			case KeyEvent.VK_S: return Communicator.KEY_S;
			case KeyEvent.VK_F: return Communicator.KEY_F;
			case KeyEvent.VK_T: return Communicator.KEY_T;
			case KeyEvent.VK_G: return Communicator.KEY_G;
			case KeyEvent.VK_Y: return Communicator.KEY_Y;
			case KeyEvent.VK_H: return Communicator.KEY_H;
			default: return Communicator.KEY_UNKNOWN;
		}
	}
	
	/**************************** CAMERA DISPLAY   ****************************/
	private Image camera;
	@Override public void update(Graphics g) { paint(g); }
	@Override public void paint(Graphics g) { g.drawImage(camera, 0, 0, this); }
	@Override public void accept(Image img) { camera = img; repaint(); }
	
	/**************************** CLIENT WINDOW    ****************************/
	private final Communicator comm = new Communicator(keys, this);
	public InputClient() {
		addKeyListener(this);
		comm.start();
	}
	
	// Camera image size
	@Override public Dimension getPreferredSize() { return new Dimension(320, 240); }
	@Override public Dimension getMinimumSize() { return new Dimension(320, 240); }
	@Override public Dimension getMaximumSize() { return new Dimension(320, 240); }
	
	// Window events
	@Override public void windowClosed(WindowEvent e) { System.exit(0); }
	@Override public void windowClosing(WindowEvent e) { comm.shutdown(); e.getWindow().dispose(); }
	@Override public void windowActivated(WindowEvent arg0) { }
	@Override public void windowDeactivated(WindowEvent arg0) { }
	@Override public void windowDeiconified(WindowEvent arg0) { }
	@Override public void windowIconified(WindowEvent arg0) { }
	@Override public void windowOpened(WindowEvent e) { }
	
	public static void main(String[] args) {
		InputClient client = new InputClient();
		Frame window = new Frame("Input Client");
		window.addKeyListener(client);
		window.addWindowListener(client);
		window.add(client);
		window.setResizable(false);
		window.pack();
		window.setVisible(true);
	}
}