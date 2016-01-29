package assignment3;

import javafx.application.Application;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.scene.control.*;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Pos;

public class A2 extends Application {
    int font_size = 20;
    @Override
    public void start(Stage primaryStage) {

        String country_names[] = {
                "Canada", "USA", "Mexico", "Brazil", "England", "Scotland", "Ireland", "Northern Ireland", "Greenland", "Iceland",
                "Germany", "Russia", "China", "Japan", "Austrailia", "New Zealand", "Italy", "Greece", "France", "Cuba"
        };

        Label lbl_countries = new Label("Countries");
        lbl_countries.setOnMouseEntered(mouseEvent -> lbl_countries.setTextFill(Color.AQUA));
        lbl_countries.setOnMouseExited(mouseEvent -> lbl_countries.setTextFill(Color.BLACK));

        Label lbl_field = new Label();
        lbl_field.setText("Text Field");
        lbl_field.setFont(new Font("Arial", font_size));

        CheckBox checkBox = new CheckBox();
        checkBox.setText("Change Colour");
        checkBox.selectedProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue == Boolean.TRUE){
                lbl_field.setStyle("-fx-background-color: yellow;");
            } else {
                lbl_field.setStyle("-fx-background-color: transparent;");
            }
        });

        Slider slider = new Slider();
        slider.setMin(10);
        slider.setMax(72);
        slider.setValue(font_size);
        slider.setShowTickLabels(true);
        slider.setShowTickMarks(true);
        slider.setMajorTickUnit(10);
        slider.valueProperty().addListener((ov, old_val, new_val) -> {
            lbl_field.setFont(new Font("Arial", (double) new_val));

        });

        ListView<String> listView = new ListView<>();
        ObservableList<String> countries = FXCollections.observableArrayList (country_names);
        listView.setItems(countries);
        listView.getSelectionModel().selectedItemProperty().addListener((observable, oldValue, newValue) -> {
            lbl_field.setText("You chose: " + newValue);
        });

        VBox root = new VBox();
        root.setAlignment(Pos.CENTER);     // Center nodes in layout

        Scene scene = new Scene(root, 300, 250);  // Window Size
        primaryStage.setTitle("Assignment 2");    // Window Title
        primaryStage.setScene(scene);


        root.getChildren().addAll(lbl_countries, listView, checkBox, new Label("Font Size"), slider, lbl_field);
        primaryStage.show();
    }


    public static void main(String[] args) {
        launch(args);
    }

}
