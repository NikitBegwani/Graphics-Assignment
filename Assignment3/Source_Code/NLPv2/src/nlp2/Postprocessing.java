/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author sachin
 */
package nlp2;
import java.util.*;
public class Postprocessing {
public String postProcessing(String s)
    {
        int i1,i2,i3,i;
        String sub3="";
        Preprocessing pr =new Preprocessing();

        i1=s.indexOf("from");
        if(s.contains("where"))
        {

                i2=s.indexOf("where");
                i3=i2+"where".length();
                sub3=s.substring(i3, s.length());


        }
        else
        {
            i2=s.length();
            i3=-1;
            sub3="";
             //return "hello";
        }
        String sub1=s.substring(0, i1+"from".length());
        String sub2=s.substring(i1+"from".length(), i2);
        String[] str=sub2.split(",") ;
        String[] strt=new String[str.length];
        StringBuilder sb = new StringBuilder();
        StringBuilder cond = new StringBuilder();
        Set<String> conCheck= new HashSet<String>();
        int l=0;
        int flag,outflag=0;
        for(int k=0;k<str.length-1&&str.length>=2;k++)
           {

               conCheck.clear();
               String s1=str[k].trim();
               String s2=str[k+1].trim();
               i=k+1;
               if(outflag==0)
               {
                   sb.append(" "+s1+" as "+"table"+i);
                   outflag=1;
                   strt[k]="table"+i;
               }
               i=i+1;
               sb.append(", "+s2+" as "+"table"+i);
               strt[k+1]="table"+i;
               conCheck.addAll(pr.hm.get(s1));
               conCheck.retainAll(pr.hm.get(s2));
               flag=0;
               for(String condition : conCheck)
               {
                   if(flag==1)
                       cond.append(" and ");
                   cond.append(strt[k].trim()+"."+condition+" = "+strt[k+1].trim()+"."+condition);
                   flag=1;
               }
               if(k<(str.length-2))
                {
                    cond.append(" and ");
                }


           }
        String newS;
        if(s.contains("where")&&str.length>=2)
        {
            newS=sub1+sb.toString()+" where "+cond+" and "+sub3;
        }
        else if(s.contains("where")&&str.length<2)
        {
            newS=sub1+sub2+"where"+sub3;
        }
        else if(!s.contains("where")&&str.length>=2)
        {
            newS=sub1+sb.toString()+" where "+cond;
        }
        else if(!s.contains("where")&&str.length<2)
        {
            newS=sub1+sub2;
        }
        else
        {
            newS=s;
        }
        //System.out.println(newS);
        return newS;
    }

}
