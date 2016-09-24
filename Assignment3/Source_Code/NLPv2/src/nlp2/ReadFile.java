/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author ASHISH
 */

package nlp2;

import java.io.*;

 public class ReadFile {
    BufferedReader br;
    String sql;
    SelectParser par;
    public ReadFile()
    {
        br=null;
        sql=null;
    }
    public String readEnglishQuery(String filename)
    {
        int result;
        try
        {    par=new SelectParser(new FileReader(filename));
             result=par.yyparse();
             if(result==0)
             {
                 br=new BufferedReader(new FileReader("/home/ashish/NetBeansProjects/NLPv2/src/nlp2/inputSql.txt"));//("/home/ashish/NetBeansProjects/NLP/src/nlp2/inputSql.txt"));
                 sql=br.readLine();
                 br.close();
             }
             else
                 sql="ERROR";

           
        }
        catch(IOException io)
        {
            io.printStackTrace();
        }
        return sql;
    }
}

