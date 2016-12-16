/**
 * Created by serena on 16/12/10.
 */
import java.io.InputStream;

import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.rdf.model.RDFNode;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.rdf.model.Statement;
import org.apache.jena.rdf.model.StmtIterator;
import org.apache.jena.util.FileManager;

public class Query2
{
    public static void main(String args[])
    {
        //String inputFileName = "E:\\Pattern Mining\\test.rdf";
        String inputFileName = "/Users/serena/desktop/_data/yagoall/yagoAll2.tsv";
        //String inputFileName = "E:\\Pattern Mining\\test.ttl";

        Model model = ModelFactory.createDefaultModel();

        InputStream in = FileManager.get().open(inputFileName);
        if (in == null)
        {
            throw new IllegalArgumentException("File: " + inputFileName + " not found");
        }

        //model.read(in, "","RDF/XML");//根据文件格式选用参数即可解析不同类型
        model.read(in, "","N3");
        //model.read(in, "","TTL");

        // list the statements in the graph
        StmtIterator iter = model.listStatements();

        // print out the predicate, subject and object of each statement
        while (iter.hasNext())
        {
            Statement stmt = iter.nextStatement(); // get next statement
            //Resource subject = stmt.getSubject(); // get the subject
            //Property predicate = stmt.getPredicate(); // get the predicate
            //RDFNode object = stmt.getObject(); // get the object

            String subject = stmt.getSubject().toString(); // get the subject
            String predicate = stmt.getPredicate().toString(); // get the predicate
            RDFNode object = stmt.getObject(); // get the object


            System.out.print("主语 " + subject);
            System.out.print(" 谓语 " + predicate);
            if (object instanceof Resource)
            {
                System.out.print(" 宾语 " + object);
            }
            else {// object is a literal  System.out.print("宾语 \"" + object.toString() + "\"");
            }
            System.out.println(" .");
        }
    }
}
