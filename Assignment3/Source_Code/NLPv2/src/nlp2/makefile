ConvertInterface:ConvertInterface.java AccessDatabase.java ReadFile.java SelectLexer.java SelectParser.java SelectParserVal.java ShowTable.java
	
SelectParser.java: SelectParser.y
		byaccj -Jclass=SelectParser -Jpackage=nlp2 SelectParser.y

SelectLexer.java: SelectLexer.l
		jflex SelectLexer.l


clean:
	rm *.class SelectLexer.java SelectParser.java
