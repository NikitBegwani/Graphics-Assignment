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
class AccessDatabase {
   /**
     * @param args the command line arguments
     */
    String sql;
    Connection con;
    ResultSet rs;
    Statement st;
    public AccessDatabase()
    {
        sql=null;
    }
    public String getSql(String filename)
    {
        ReadFile rf=new ReadFile();
        sql=rf.readEnglishQuery(filename);
/* my change*/
        if(sql=="ERROR")
            return sql;
        else
        {
                sql +=";";
                return sql;
        }
    }
    public void fireQuery(String query) {
        // TODO code application logic here
       //Test t=new Test();
        try
        {
            Class.forName("com.mysql.jdbc.Driver");
        }
        catch(Exception e)
        {
            System.out.println("registering error");
        }
        try
        {   con=DriverManager.getConnection("jdbc:mysql://localhost/students","root","ashish");
            st=con.createStatement();
            //sql=t.getSql();
            rs=st.executeQuery(query);//("select current_schema from   sysibm.sysdummy1");
            ShowTable show = new ShowTable(rs);
            
            /*ResultSetMetaData md = rs.getMetaData();
            for( int i = 1; i <= md.getColumnCount(); i++ )
                System.out.print( md.getColumnLabel(i) + "\t" ) ;
            System.out.println() ;

            while(rs.next())
            {
                for( int i = 1; i <= md.getColumnCount(); i++ )
                    System.out.print( rs.getString(i) + "\t" ) ;
                System.out.println() ;
         
            }*/
            st.close();
            con.close();
            System.out.println("Query Completed");
        }
        catch(Exception e)
        {
            e.printStackTrace();
            //System.out.println("execution error");
        }
    }
}
