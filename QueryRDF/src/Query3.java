import org.apache.jena.query.*;
import org.apache.jena.tdb.TDBFactory;

import java.io.*;

/**
 * Created by serena on 16/12/10.
 */
public class Query3 {
    public static void main(String... argv) {
        String directory = "/Users/serena/desktop/yago";
        Dataset dataset = TDBFactory.createDataset(directory);
        String rdf = "http://www.w3.org/1999/02/22-rdf-syntax-ns#";
        String base = "http://yago-knowledge.org/resource/";
        // Start READ transaction.
        //   No updates or changes to the dataset are possible while this
        //   dataset is used for a read transaction.
        //   An application can have other Datasets, in the same JVM,
        //   tied to the same TDB database performing read or write
        //   transactions concurrently.

        dataset.begin(ReadWrite.READ);
            // Do some queries
            /*String sparqlQueryString1 = "SELECT ?d ?c ?a WHERE { ?d " + "<" + base + "isMarriedTo>" + " ?c  . " + "\n"
                    + "?d " + "<" + base + "isLeaderOf>" + " ?a    ." + "\n"
        + "?a <" + rdf+ "type> " + "<" + base +"wordnet_country_108544813>    ." + "\n" +
                    "?c <"+ rdf + "type> " + "<" + base + "wordnet_first_lady_110092880> .}" + "\n";*/
            String sparqlQueryString1 = "PREFIX rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#>" + "\n"
                    + "PREFIX base: <http://yago-knowledge.org/resource/>" + "\n";
        sparqlQueryString1 += "SELECT ?d ?c ?a WHERE { ?d base:isMarriedTo ?c  . " + "\n"
                + "?d base:isLeaderOf ?a    ." + "\n"
                + "?a rdf:type base:wordnet_country_108544813    ." + "\n" +
                "?c rdf:type base:wordnet_first_lady_110092880 ." + "\n" + "OPTIONAL {?a base:dealsWith base:Japan}" +
        "\n" + "}";
        System.out.println(sparqlQueryString1.toString());
        Query query = QueryFactory.create(sparqlQueryString1) ;
        QueryExecution qexec = QueryExecutionFactory.create(query, dataset) ;
        ResultSet results = qexec.execSelect() ;

        File out = new File("out.txt");
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


        ResultSetFormatter.out(outf,results);

        //ResultSetFormatter.out(results) ;
        qexec.close() ;

        dataset.close();
    }
}
