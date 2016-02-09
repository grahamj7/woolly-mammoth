package Assignment4;

import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;
import javafx.scene.canvas.Canvas;
import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class GraphMiniMapView extends Canvas {

    GraphicsContext graphicsContext;
    private ObservableList<GraphNode> nodes;
    private ObservableList<GraphEdge> edges;

    public GraphMiniMapView(Graph g, double width, double height) {
        super(width, height);
        graphicsContext = this.getGraphicsContext2D();

        nodes = g.getNodes();
        edges = g.getEdges();

        edges.addListener((ListChangeListener<GraphEdge>) c -> redraw());
        nodes.addListener((ListChangeListener<GraphNode>) c -> redraw());

    }

    private void redraw(){
        graphicsContext.clearRect(0, 0, this.getWidth(), this.getHeight());
        for(GraphEdge edge: edges){
            drawEdge(edge.getN1().getX()*getWidth()+5, edge.getN1().getY()*getHeight()+5,edge.getN2().getX()*getWidth()+5, edge.getN2().getY()*getHeight()+5);
        }

        for(GraphNode node : nodes){
            drawNode(node.getX()*getWidth(), node.getY()*getHeight());
        }
    }

    private void drawNode(double x, double y)
    {
        graphicsContext.restore();
        graphicsContext.setFill(Color.BLACK);
        graphicsContext.fillOval(x, y, 10, 10);


        graphicsContext.fill();
    }

    private void drawEdge(double startX, double startY, double endX, double endY)
    {
        graphicsContext.restore();
        graphicsContext.setStroke(Color.BLACK);
        graphicsContext.strokeLine(startX, startY, endX, endY);

        graphicsContext.stroke();
    }
}
