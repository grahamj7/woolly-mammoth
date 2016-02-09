package Assignment4;

//A simple edge class which contains two nodes which the edge connects
public class GraphEdge {
    
    private GraphNode n1;
    private GraphNode n2;

    public GraphEdge(GraphNode V1, GraphNode V2)
    {
        n1 = V1;
        n2 = V2;
    }
    
    public GraphNode getN1() {
        return n1;
    }

    public GraphNode getN2() {
        return n2;
    }

}
