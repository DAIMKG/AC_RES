/**
 * Created by serena on 16/12/10.
 */
import org.apache.jena.query.Dataset ;
import org.apache.jena.query.Query ;
import org.apache.jena.query.QueryExecution ;
import org.apache.jena.query.QueryExecutionFactory ;
import org.apache.jena.query.QueryFactory ;
import org.apache.jena.query.QuerySolution ;
import org.apache.jena.query.ReadWrite ;
import org.apache.jena.query.ResultSet ;
import org.apache.jena.tdb.TDBFactory ;

public class Query1
{
    public static void main(String... argv)
    {
        String directory = "/Users/serena/desktop/yago" ;
        Dataset dataset = TDBFactory.createDataset(directory) ;
        String rdf = "<http://www.w3.org/1999/02/22-rdf-syntax-ns#>";
        // Start READ transaction.
        //   No updates or changes to the dataset are possible while this
        //   dataset is used for a read transaction.
        //   An application can have other Datasets, in the same JVM,
        //   tied to the same TDB database performing read or write
        //   transactions concurrently.

        dataset.begin(ReadWrite.READ) ;
        try
        {
            // Do some queries
            String sparqlQueryString1 = "SELECT (?d ?c ?a) " +
                    " where { ?d <isMarriedTo> ?c   ." +
                    " ?d <isPoliticianOf> ?a    ." +
                    "?a rdf:type <wordnet_country_108544813>    ." +
                    "?c rdf:type <wordnet_first_lady_110092880> .}" ;
            execQuery(sparqlQueryString1, dataset) ;

            //String sparqlQueryString2 = "SELECT * { ?s ?p ?o }" ;
            //execQuery(sparqlQueryString2, dataset) ;

            // Can also call dataset.abort() or dataset.commit() here
        }
        finally
        {
            // Notify the end of the READ transaction.
            // Any use of dataset.abort() or dataset.commit() or dataset.end()
            // .end() can be called multiple times for the same .begin(READ)
            dataset.end() ;
        }
    }

    public static void execQuery(String sparqlQueryString, Dataset dataset)
    {
        Query query = QueryFactory.create(sparqlQueryString) ;
        QueryExecution qexec = QueryExecutionFactory.create(query, dataset) ;
        try {
            ResultSet results = qexec.execSelect() ;
            for ( ; results.hasNext() ; )
            {
                QuerySolution soln = results.nextSolution() ;
                //int count = 3;
                int count = soln.getLiteral("count").getInt() ;

                System.out.println("count = "+count) ;
            }
        } finally { qexec.close() ; }
    }
}
