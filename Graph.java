package Assignment4;

import java.util.ArrayList;
import javafx.beans.Observable;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;
import javafx.util.Callback;


//A simple graph class consisting of a collection of nodes and a collection of edges
public class Graph {
    
    //Using ObservableLists for to hold the nodes and edges so that the view can listen
    //for changes
    private final ObservableList<GraphNode> nodes;
    private final ObservableList<GraphEdge> edges;
    
    public Graph()
    {
        //Initialize the collection of nodes
        //By providing a callback, we can tell the list to listen for changes to the specified properties
        //and fire update events when they change
        nodes = FXCollections.observableArrayList(new Callback<GraphNode,Observable[]>() {
            @Override
            public Observable[] call(GraphNode p) {
                return new Observable[] {p.textProperty(), p.xProperty(), p.yProperty()};
            }
        });
        
        //Initialize the collection of edges
        ArrayList<GraphEdge> eal = new ArrayList<>();
        edges = FXCollections.observableArrayList(eal);
        
        //When the list of nodes changes, check to see that if any nodes were removed,
        //any edges which contain that node are removed as well
        nodes.addListener((ListChangeListener<GraphNode>) c -> {
            while (c.next()) {
                for (GraphNode remitem : c.getRemoved()) {
                    for (int i = 0; i <edges.size(); i++)
                    {
                        GraphEdge currEdge = edges.get(i);
                        if (currEdge.getN1() == remitem || currEdge.getN2() == remitem)
                        {
                            edges.remove(i);
                            i--;
                        }
                    }
                }
            }
        });
    }
    

    //getter for the list of nodes
    public ObservableList<GraphNode> getNodes() {
        return nodes;
    }

    //getter for the list of edges
    public ObservableList<GraphEdge> getEdges() {
        return edges;
    }
}
