/**
 * Created by serena on 16/12/17.
 */
/**
 * Created by serena on 16/12/17.
 */
import org.apache.jena.query.*;
import org.apache.jena.tdb.TDBFactory;

import java.io.*;
public class Count_sp
{
    public static void main(String... argv){
        String directory = "/Users/serena/desktop/yago";
        Dataset dataset = TDBFactory.createDataset(directory);
        String rdf = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
        String base = "http://yago-knowledge.org/resource/";

        dataset.begin(ReadWrite.READ);
        // gc overhead limit ,give up this way
        String sparqlQueryString1 = "PREFIX rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#>" + "\n"
                + "PREFIX base: <http://yago-knowledge.org/resource/>" + "\n";
        sparqlQueryString1 += "SELECT ?s ?p (count(*) as ?count) WHERE { ?s ?p ?o    . " +
                "}\n" + "GROUP BY ?s ?p";
        System.out.println(sparqlQueryString1.toString());
        Query query = QueryFactory.create(sparqlQueryString1) ;
        QueryExecution qexec = QueryExecutionFactory.create(query, dataset) ;
        ResultSet results = qexec.execSelect() ;

        File out = new File("statis_sp.csv");
        OutputStream outf = null;
        try {
            out.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            outf = new FileOutputStream(out);

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }


        ResultSetFormatter.outputAsCSV(outf,results);

        //ResultSetFormatter.out(results) ;
        qexec.close() ;

        dataset.close();
    }
}

