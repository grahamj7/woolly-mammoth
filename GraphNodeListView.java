package Assignment4;

import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.layout.VBox;

public class GraphNodeListView extends VBox {
    TextField textBox;
    ListView<GraphNode> listView;

    public GraphNodeListView() {
        textBox = new TextField();
        textBox.setOnAction(event -> {
            listView.getSelectionModel().getSelectedItem().setText(textBox.getText());
            listView.refresh();
        });

        listView = new ListView<>();
        listView.getSelectionModel().selectedItemProperty().addListener((observable, oldValue, newValue) -> {
            textBox.setText(listView.getSelectionModel().getSelectedItem().getText());
        });

        this.getChildren().addAll(textBox, listView);
    }

}
