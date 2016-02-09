package Assignment4;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.scene.layout.*;
import javafx.stage.Stage;


public class MVCEditor extends Application {

    @Override
    public void start(Stage primaryStage) {
        int width = 1000;
        int height = 1000;

        BorderPane root = new BorderPane();
        VBox left = new VBox();

        Graph g = new Graph();
        GraphMiniMapView minimap = new GraphMiniMapView(g, width*.25, height*.25);
        GraphNodeListView nodeListView = new GraphNodeListView();
        GraphView gv = new GraphView(g, nodeListView.listView, width*.75, height*.75);

        GraphNode n1 = new GraphNode(0.25, 0.25);
        GraphNode n2 = new GraphNode(0.75, 0.75);
        GraphEdge e = new GraphEdge(n1, n2);

        g.getNodes().add(n1);
        g.getNodes().add(n2);
        g.getEdges().add(e);

        nodeListView.listView.getSelectionModel().select(0);
        left.getChildren().addAll(minimap, nodeListView);
        root.setLeft(left);
        root.setCenter(gv);

        Scene scene = new Scene(root, width, height);
        primaryStage.setTitle("Assignment 4");
        primaryStage.setScene(scene);
        primaryStage.show();
    }



    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
    
}
