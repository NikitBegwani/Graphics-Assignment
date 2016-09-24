%{
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author ASHISH
 */
import java.io.*;
import java.util.StringTokenizer;
import javax.swing.*;
%}

/* YACC Declarations */

%token SELECT
%token TN
%token CN
%token FROM
%token WHERE
%token COND
%token NUM
%token EOLN

/* Grammar Rules */

%%
sqls:
	sql					{
							try
							{
								BufferedWriter br = new BufferedWriter(new FileWriter("/home/ashish/NetBeansProjects/NLPv2/src/nlp2/inputSql.txt"));
								br.write($1.sval,0,$1.sval.length());
								br.close();
							}
							catch(IOException io)
							{
								io.printStackTrace();
							}
						}
	;
sql: 
	selectClause fromClause				{$$.sval=$1.sval+" "+$2.sval;System.out.println($$.sval);}
	|selectClause fromClause EOLN			{$$.sval=$1.sval+" "+$2.sval;System.out.println($$.sval);}
	|selectClause fromClause whereClause EOLN	{$$.sval=$1.sval+" "+$2.sval+" "+$3.sval;System.out.println($$.sval);}
	;

select:
	SELECT 	{$$.sval="select distinct";}//System.out.println($$.sval);}
	;

Column:
	CN	{$$.sval=$1.sval;}
	;

ColumnList:
	ColumnList Column	{$$.sval=$1.sval+" "+$2.sval;}
	|Column			{$$.sval=$1.sval;}
	;

selectClause:
	select ColumnList	{$$.sval=$1.sval+" "+$2.sval;}
	|select			{$$.sval=$1.sval+" *";}//System.out.println($$.sval);}
	;

from:
	FROM	{$$.sval="from";}//System.out.println($$.sval);}
	;

Table:
	TN	{$$.sval=$1.sval;}//System.out.println($$.sval);}
	;

TableList:
	TableList Table	{$$.sval=$1.sval+" "+$2.sval;}
	|Table	{$$.sval=$1.sval;}//System.out.println($$.sval);}
	;

fromClause:
	from TableList		{$$.sval=$1.sval+" "+$2.sval;}//System.out.println($$.sval);}
	|TableList		{$$.sval="from ";$$.sval+=$1.sval;}//System.out.println($$.sval);}
	;

where:
	WHERE	{$$.sval=$1.sval;}
	;

condition:
	COND	{$$.sval=$1.sval;}
	;

value:
	NUM	{$$.sval=$1.sval;}
	;

whereClause:
	where CN condition value	{$$.sval=$1.sval+" "+$2.sval+" "+$3.sval+" "+$4.sval;}
	|where CN value			{$$.sval=$1.sval+" "+$2.sval+" = "+$3.sval;}
	;

%%
/* reference to the lexer object */
private SelectLexer lexer;
private int flagParse=0;
/* Byacc/J expects a member method int yylex(). We need to provide one
   through this mechanism. See the jflex manual for more information. */

	/* interface to the lexer */
private int yylex() 
{
		int retVal = -1;
		try {
			retVal = lexer.yylex();
		} catch (IOException e) {
			System.err.println("IO Error:" + e);
		}
		
		return retVal;
}

public void yyerror (String error) {
	if(flagParse==0)
	{	
		flagParse=1;
		System.err.println("Error : " + error + " at line " + lexer.getLine());
		System.err.println("String rejected");
		String message="Sorry! Not able to convert the given text into SQL\nTry to enter the text in another form";
                JOptionPane.showMessageDialog(null, message,"ERROR!" , JOptionPane.ERROR_MESSAGE);
		//System.exit(0);
	}
}


public SelectParser (FileReader r) {
		lexer = new SelectLexer(r, this);
	}


/*public static void main(String args[]) throws IOException
{
	Parser par=new Parser(new FileReader("in.txt"));
	par.yyparse();
}*/

