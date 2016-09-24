/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author ASHISH
 */
package nlp2;
import java.sql.*;
import javax.swing.*;
import java.awt.event.*;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Vector;

public class ConvertInterface
{
        private JLabel label1,label2,label3;
	private JButton button1,button2,button3;
	private JTextArea textArea1,textArea2;
        private ImageIcon ii;
        String engText,sqlText;
        AccessDatabase t;
        Postprocessing post;
        Preprocessing pre;
    public ConvertInterface() {
        JFrame frame1 = new JFrame("Natural Language To SQL Converter");
        frame1.setSize(800, 600);
        frame1.setLayout(null);
        frame1.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame1.setVisible(true);
      
        //Labels Definitions
        ii = new ImageIcon("/home/ashish/NetBeansProjects/NLPv2/src/nlp2/img4.png");
        label1 = new JLabel(ii);
        frame1.add(label1);
        label1.setBounds(0, 10, 790, 150);
        label2 = new JLabel("Enter english text here...");
        frame1.add(label2);
        label2.setBounds(10, 170, 190, 14);
        label3 = new JLabel("SQL corresponding to text");
        frame1.add(label3);
        label3.setBounds(10, 340, 240, 14);
        
        //Buttons Definitions
        button1 = new JButton("Generate SQL");
        frame1.add(button1);
        button1.setBounds(50,300,150,23);
        button2 = new JButton("Show SQL");
        frame1.add(button2);
        button2.setBounds(300, 300, 150, 23);
        button3 = new JButton("Fire Query");
        frame1.add(button3);
        button3.setBounds(550, 300, 150, 23);
        button3.setEnabled(false);
        
        //textArea definitions
        textArea1 = new JTextArea(5, 100);
        frame1.add(textArea1);
        textArea1.setBounds(10, 190, 760, 76);
        textArea1.setLineWrap(true);
        textArea2 = new JTextArea(5, 100);
        frame1.add(textArea2);
        textArea2.setBounds(10, 360, 760, 76);
        textArea2.setLineWrap(true);
        
        // handling Events
        button1.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
                engText = textArea1.getText();
                textArea2.setText("");
                pre = new Preprocessing();
                post = new Postprocessing();
                pre.createjoin();
                String newEngText = pre.getSt(engText); System.out.println(" New Formatted: "+newEngText);
                newEngText +="\n";
                String filename = "/home/ashish/NetBeansProjects/NLPv2/src/nlp2/inputEng.txt";
                try
                {
                       BufferedWriter br = new BufferedWriter(new FileWriter(filename));
                       br.write(newEngText);
                       br.close();
                }
                catch(IOException io)
                {
                    io.printStackTrace();
                }
                t = new AccessDatabase();
                
                sqlText=t.getSql(filename);
                if(sqlText!="ERROR")
                {
                    button3.setEnabled(true);
                    //System.out.println(sqlText);
                    sqlText = post.postProcessing(sqlText);
                }
               /* if(sqlText=="SERROR")
                {System.out.println(sqlText);
                    String message="Sorry! Not able to convert the given text into SQL\nTry to enter the text in another form";
                    JOptionPane.showMessageDialog(null, message,"ERROR!" , JOptionPane.ERROR_MESSAGE);
                }
                else if(sqlText=="WERROR")
                {
                    String message="unknown word! Please enter the correct name of attribute or any vocabulary word";
                    JOptionPane.showMessageDialog(null, message,"ERROR!" , JOptionPane.ERROR_MESSAGE);
                }*/
            }
        });
        button2.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
                textArea2.setText(sqlText);
            }
        });
	button3.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e)
            {
            	//System.out.println(sqlText);
            	t.fireQuery(sqlText);
                button3.setEnabled(false);
            }
        });
        
    }
    
    public static void main(String[] args) 
    {
        
        try {
		javax.swing.UIManager.setLookAndFeel("com.birosoft.liquid.LiquidLookAndFeel");

	    // if you want decorations for frames and dialogs you can put this two lines
	    //
		// JFrame.setDefaultLookAndFeelDecorated(true);	// to decorate frames 
		// JDialog.setDefaultLookAndFeelDecorated(true);	// to decorate dialogs 
	    //
	    // or put this one line
	    //
		// com.birosoft.liquid.LiquidLookAndFeel.setLiquidDecorations(true);
		//
		// or if you want to use Apple's Panther window decoration
		//
		// com.birosoft.liquid.LiquidLookAndFeel.setLiquidDecorations(true, "panther");

    } catch (Exception e) {}
        
        
        SwingUtilities.invokeLater(new Runnable(){
            public void run()
            {
                new ConvertInterface();
            }
        });
    }
    
}
