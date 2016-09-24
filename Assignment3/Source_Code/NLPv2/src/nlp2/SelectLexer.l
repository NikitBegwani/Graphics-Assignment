/**
 *
 * @author ASHISH
 */
package nlp2;
import java.io.*;
import javax.swing.*;
%%
%class SelectLexer
%unicode
%int
%line
%column
%byaccj

%{
	private SelectParser yyparser;
	int flagLex=0;
	public SelectLexer ( FileReader r, SelectParser yyparser) 
	{
		this(r);	
		this.yyparser = yyparser;
	}
	
	public int getLine() 
	{
		return yyline;
	}
	public void yyerror(String err)
	{
		if(flagLex==0)
		{	
			flagLex=1;
			String message="unknown word "+"\""+err+"\""+"! Please enter the correct name of attribute or any vocabulary word";
	                JOptionPane.showMessageDialog(null, message,"ERROR!" , JOptionPane.ERROR_MESSAGE);
			//System.exit(0);
		}
		//System.out.println(err);
	}
	public void endLex() 
	{
		try
		{
			yyclose();
		}
		catch(IOException io)
		{
			io.printStackTrace();
		}
	}
	
	
%}

%%
[Gg]et|[Ff]ind|[Ss]elect|[Ll]ist|[Ss]how|[Ww]hich|[Gg]ive	 					{return SelectParser.SELECT;}
(Account|Branch|Customer|Depositor|Loan|Borrower|[Dd]epartment),?		{yyparser.yylval=new SelectParserVal(yytext());return SelectParser.TN;}
[Ff]rom|[Oo]f												{return SelectParser.FROM;}
([a-z]+_name|[a-z]+_(city|cities)|[a-z]+_number|balance|assets|customer_street|loan_amount|[Dd]eptno),?	{yyparser.yylval=new SelectParserVal(yytext());return SelectParser.CN;}
all|the|and										{;}
"is greater than"						{yyparser.yylval=new SelectParserVal(">");return SelectParser.COND;}
"is greater than equal to"					{yyparser.yylval=new SelectParserVal(">=");return SelectParser.COND;}
"is less than"							{yyparser.yylval=new SelectParserVal("<");return SelectParser.COND;}
"is less than equal to"						{yyparser.yylval=new SelectParserVal("<=");return SelectParser.COND;}
"is equal to"|"is"						{yyparser.yylval=new SelectParserVal("=");return SelectParser.COND;}
where|whose|at|in						{yyparser.yylval=new SelectParserVal("where");return SelectParser.WHERE;}
[0-9]+								{yyparser.yylval=new SelectParserVal(yytext());return SelectParser.NUM;}
[a-zA-Z]+							{yyparser.yylval=new SelectParserVal("\""+yytext()+"\"");return SelectParser.NUM;}
\r?\n								{endLex();return SelectParser.EOLN;}
[ \t]								{;}	/* skip white spaces*/
.								{yyerror(yytext());}


