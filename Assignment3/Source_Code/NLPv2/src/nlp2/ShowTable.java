/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author ASHISH
 */
package nlp2;

import java.awt.BorderLayout;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.sql.*;
import java.util.Vector;
import javax.swing.*;

public class ShowTable extends JPanel{
    private JTable table;
    public ShowTable(ResultSet rs) throws SQLException
    {
        JFrame f= new JFrame("Result of Query");
        ShowResult(rs);
        setSize(450,200);
        setVisible(true);
        f.add(this);
        f.setSize(500,500);
        f.setVisible(true);
        f.addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e)
            {
                setVisible(false);
            }
        });
    }
    public void ShowResult(ResultSet rs) throws SQLException
    {
         //position to first record
        boolean moreRecords= rs.next();
        
        // if there are no records, display a message
        if(!moreRecords)
        {
            JOptionPane.showMessageDialog(null, "ResultSet Contained no records");
            return;
        }
        Vector columns = new Vector();
        Vector rows = new Vector();
        try
        {
            // get columns
            ResultSetMetaData rsmd = rs.getMetaData();
            for(int i=1;i<=rsmd.getColumnCount();i++)
                columns.addElement(rsmd.getColumnName(i));
            // get row data
            do
            {
                rows.addElement(getNextRow(rs,rsmd));
            }while(rs.next());
            
            // display table with ResultSet contents
            table = new JTable(rows,columns);
            JScrollPane scroller = new JScrollPane(table);
            setLayout(new BorderLayout());
            add(scroller,BorderLayout.CENTER);
            validate();
        }
        catch(SQLException sqlex)
        {
            sqlex.printStackTrace();
        }
    }
    public Vector getNextRow(ResultSet rs, ResultSetMetaData rsmd) throws SQLException
    {
        Vector currentRow = new Vector();
        for(int i=1;i<=rsmd.getColumnCount();i++)
            switch(rsmd.getColumnType(i))
            {
                case Types.VARCHAR:
                                        currentRow.addElement(rs.getString(i));
                                        break;
                case Types.INTEGER:
                                        currentRow.addElement(rs.getLong(i));
                                        break;
                case Types.CHAR:
                                        currentRow.addElement(rs.getString(i));
                                        break;
                default:
                                        System.out.println("Type was: "+rsmd.getColumnTypeName(i));
            }
        return currentRow;
            
    }
}
