package su.hirakilab;

import java.io.*;

public class Caesar {
	private InputStream m_in;
	private OutputStream m_out;

	public Caesar(InputStream i, OutputStream o) {
		m_in=i;
		m_out=o;
	}

	public void run() throws IOException {
		int c;
		while(0<(c=m_in.read())) {
			if(Character.isLetterOrDigit(c)) { c++; }
			m_out.write((char)c);
		}
	}

	public static void main(String[] args) {
		InputStream i=System.in;
		OutputStream o=System.out;
		try {
			if(args.length>0) {
				if(!("-".equals(args[0]))) {
					i=new FileInputStream(args[0]);
				}
			}
			if(args.length>1) {
				o=new FileOutputStream(args[1]);
			}
		} catch (Exception e) {
			System.out.println(e.toString());
			if(i!=null) {
				try { i.close(); } catch (IOException ignored) {}
			}
			System.exit(1);
		}

		Caesar c=new Caesar(i,o);
		try {
			c.run();
		} catch (IOException e) {
			System.err.println(e.toString());
		} finally {
			try { o.close(); } catch (IOException ignored) {}
			try { i.close(); } catch (IOException ignored) {}
		}
	}
}
