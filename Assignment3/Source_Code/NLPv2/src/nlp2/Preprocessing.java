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
import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
public class Preprocessing {
    public static ArrayList<Set<String>> Ar=new ArrayList<Set<String>>();
    public static HashMap<String, Set> hm=new HashMap<String, Set>();
    public static HashMap<String, String> hmp=new HashMap<String, String>();
    public static HashMap<Set, Set> hmt = new HashMap<Set, Set>();
    public static HashMap<String, String> hmr = new HashMap<String, String>();
    public static Set<String> atrunion= new HashSet<String>();
    public static ArrayList<String> nl=new ArrayList<String>();
    public static int flg=0;
    public static int count;
    public static int c;
    public void createjoin(){
        File file = new File("/home/ashish/NetBeansProjects/NLPv2/src/nlp2/DBschema.txt");
        FileInputStream fis = null;
        BufferedInputStream bis = null;
        DataInputStream dis = null;
        try {
                fis = new FileInputStream(file);


                bis = new BufferedInputStream(fis);
                dis = new DataInputStream(bis);
                while (dis.available() != 0) {

                String s1=dis.readLine();
                String[]token=s1.split(":");
                String[] token1=token[1].split(",");
                SortedSet<String> st1 = new TreeSet<String>(Arrays.asList(token1));
                hm.put(token[0],st1);
                }
                fis.close();
                bis.close();
                dis.close();

            }
          catch (FileNotFoundException e) {
                e.printStackTrace();
            }
          catch (IOException e) {
                e.printStackTrace();
            }
        Preprocessing t=new Preprocessing();
        String[] members = { "Branch","Customer","Loan","Borrower","Account","Depositor" };
        ArrayList<String> set = new ArrayList<String>();
        ArrayList<String> set1= new ArrayList<String>();
        ArrayList<String> set4= new ArrayList<String>();
        for (String n : members) {
			if (!set.contains(n)) {
				set.add(n);
			}
		}
        Ar.clear();
        int ini=0,limit=0,prev=0;
        for(int r=1;r<=6;r++)
        {
            count=0;
            t.comb(members,set1,0,7,r,r);
            prev=ini;
            ini=ini+limit;
            limit=count;
            if(r>=2)
                t.tableJ(prev,ini,r,set);
        }

            findunion();
            hmp.put("branch_name","Branch");
            hmp.put("account_number","Account");
            hmp.put("loan_number","Loan");
            hmp.put("customer_name","Customer");



     }
    public void comb(String[]s1, ArrayList<String> s2,int i, int n, int r, int r1)
    {
            int k,l;
            l=s2.size();
            if(l==r1)
            {    count++;
                 Set<String> ad= new HashSet<String>(s2);
                 Ar.add(ad);
                 if(l==1)
                 {
                     SortedSet<String> set1= new TreeSet<String>();
                     SortedSet<String> set2= new TreeSet<String>();
                     set1.addAll(s2);
                     set2.addAll(hm.get(s2.get(0)));
                     hmt.put(set1, set2);
                 }
            }
            else
            {
                for(k=i;k<n-r;k++)
                {
                    l=s2.size();
                    s2.add(s1[k]);
                    comb(s1,s2,k+1,n,r-1,r1);
                    s2.remove(l);
                }
            }
    }
    public void tableJ(int i, int limit,int r,ArrayList<String> al1)
    {

        SortedSet<String> set= new TreeSet<String>();
        SortedSet<String> iner1= new TreeSet<String>();
        SortedSet<String> iner2= new TreeSet<String>();
        for(int j=i;j<limit;j++)
        {
            set.clear();
            set.addAll(Ar.get(j));
            if(hmt.containsKey(set))
            {

                 for(int k=0;k<al1.size();k++)
                 {
                     iner1.clear();
                     iner1.add(al1.get(k));
                     iner1.retainAll(set);
                     if(iner1.size()==0)
                     {
                         SortedSet<String> u1= new TreeSet<String>();
                         SortedSet<String> u2= new TreeSet<String>();
                         u1.clear();
                         u2.clear();
                         u1.addAll(hmt.get(set));
                         u2.addAll(hm.get(al1.get(k)));
                         iner2.clear();
                         iner2.addAll(u1);
                         iner2.retainAll(u2);
                         if(iner2.size()>0)
                            {
                                SortedSet<String> setut= new TreeSet<String>();
                                u1.addAll(u2);
                                setut.clear();
                                setut.addAll(set);
                                setut.add(al1.get(k));
                                hmt.put(setut, u1);

                            }
                     }
                 }
            }
        }
    }
       public ArrayList Search(ArrayList<String> str)
       {

           SortedSet<String> sset=new TreeSet<String>(str);
           SortedSet<String> ssett=new TreeSet<String>();
           ArrayList ret=new ArrayList();
           Set sett = hmt.entrySet();
           int size=100;
        // Get an iterator
           Iterator i = sett.iterator();
        // Display elements
           int kount=0;
           //System.out.println("in search "+sset+" ");
           while(i.hasNext()) {kount++;
                Map.Entry me = (Map.Entry)i.next();
                ssett.addAll((Set)me.getValue());
                if(ssett.containsAll(sset))
                {
                    if(ssett.size()<size)
                    {
                        ret.clear();
                        ret.add(me.getKey());
                        size=ssett.size();
                    }
                }
                ssett.clear();
            }
           return ret;

       }
       public static void findunion()
       {
           Set set = hm.entrySet();
        // Get an iterator
        Iterator i = set.iterator();
        while(i.hasNext()) {
                Map.Entry me = (Map.Entry)i.next();
                atrunion.addAll((Set)me.getValue());

            }
       }
    public String getSt(String s)
    {   if(s.contains("from"))
        {
            //System.out.println(" c "+s);
            return s;

        }
        //System.out.println(" ac "+s);
        String[] str= s.split(" |,");
        ArrayList<String> AL= new ArrayList<String>();
        AL.clear();
        for(String token: str){
			if (!AL.contains(token)) {
				AL.add(token);
			}
		}

        Set<String> set= new HashSet<String>(AL);
        Set<String> set1= new HashSet<String>();

        set1.addAll(set);
        set1.retainAll(atrunion);

       //System.out.println("intersection set "+set1+" "+set);
        ArrayList<String> Attr=new ArrayList<String>(set1);
        //System.out.println(" ATTR "+Attr);
        ArrayList<String> tables=new ArrayList<String>();
        int flag=0;
        if(Attr.size()==1)
        {
            //System.out.println(" attr size 1");
            String attr=Attr.get(0);
            //System.out.println(" attr "+attr);
            if(hmp.containsKey(attr.trim()))
            {
                //System.out.println(" found ");
                String string= s+" from "+hmp.get(attr.trim()).trim();
                return string;


            }
        }
        tables.clear();
        tables.addAll(this.Search(Attr));

        //System.out.println("tables "+tables);
        String[] token={"where","Where","whose","Whose","at"};
        Set<String> st1 = new HashSet<String>(Arrays.asList(token));
        String[] str1= s.split(" ");
        String str2=tables.toString().replaceAll("\\[|\\]", " ");
        //System.out.println(" table after "+str2);
        //nl.clear();
        flg=0;
        this.nJoin(str2);
        //System.out.println(" b4 nl "+str2+" aftr nl "+nl);
        StringBuilder sb2=new StringBuilder();

        for(int x=0;x<nl.size();x++)
        {
            sb2.append(nl.get(x));
            if(x!=(nl.size()-1))
            {
                sb2.append(", ");
            }
        }
        //System.out.println(" sb2_table "+sb2);
        StringBuilder sb = new StringBuilder();
        int f=0;
         for(String token1: str1){
			if (st1.contains(token1)) {
				//sb.append("from "+tables.toString().replaceAll("\\[|\\]", "")+" "+token1+" ");
                                sb.append("from"+sb2+" "+token1+" ");
                                f=1;
			}
                        else
                        {
                            sb.append(token1+" ");
                        }
		}
         if(f==0)
         {
            // sb.append("from "+tables.toString().replaceAll("\\[|\\]", ""));
             sb.append("from"+sb2);
         }
         //System.out.println("query "+sb);
         return sb.toString();
    }
    public void nJoin(String str)
    {   //System.out.println(" in njoin "+str);
        String[] tokens= str.split(",") ;
        ArrayList<String> retn=new ArrayList<String>(Arrays.asList(tokens));
        ArrayList<String> ret=new ArrayList<String>();
        if(tokens.length==1)
        {
            nl.clear();
            nl.addAll(Arrays.asList(tokens));
        }

        else if(tokens.length==2)
        {
            nl.clear();
            nl.addAll(Arrays.asList(tokens));
        }
        else{
             //System.out.println(" going to p "+retn.size());
             this.permute(retn, 0, retn.size());
        }
    }

    public void permute(ArrayList<String> str,int i,int n)
    {
        int j,l=0;
        Set<String> un= new HashSet<String>();

        String temp;
        if(i == n)
        {  //System.out.println(" perm "+str);
           for(int k=0;k<n-1;k++)
           {
               un.clear();
               String s1=str.get(k).trim();
               //System.out.println(s1+" : "+hm.get(s1));
               String s2=str.get(k+1).trim();
               //System.out.println(s2+" : "+hm.get(s2));
               un.addAll(hm.get(s1));
               un.retainAll(hm.get(s2));
               if(un.size()>0)
               {
                   l++;
                   //System.out.println(" l "+l);
               }
               else
                   continue;

           }
           if(l==n-1)
           {   flg=1;
               nl.clear();
               nl.addAll(str);
               return;
           }
           //return str;
        }

        else if(flg==0)
            {
            for (j = i; j <n&&(flg==0); j++)
                {
                temp=str.get(i);
                str.set(i, str.get(j));
                str.set(j, temp);
                permute(str, i+1, n);
                temp=str.get(i);
                str.set(i, str.get(j));
                str.set(j, temp);
            }

       }
        return;
    }
}


