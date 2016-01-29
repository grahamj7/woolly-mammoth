package assignment3;


import javafx.application.Application;

import javafx.collections.FXCollections;

import javafx.collections.ObservableList;
import javafx.collections.transformation.SortedList;
import javafx.geometry.Insets;

import javafx.scene.effect.DropShadow;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;

import javafx.stage.Stage;
import javafx.stage.FileChooser;

import java.io.File;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.Map;


public class A3 extends Application {

    MyButton button1, button2, button3, button4, button5, button6;
    Button save, quit, store, remove;
    CheckBox africa, asia, n_america, s_america, europe, australia;
    Stage stage;
    TextArea message_log, description_area;
    ImageView flag;
    Label country_label;
    ListView<String> listview;
    SortedList<String> sortedList;
    ObservableList<String> country_list;

    public static String[] getCountryNames() {
        String[] names = new String[202];
        int i = 0;
        for (Country country: Country.getCountries()){
            names[i] = country.Name;
            i++;
        }
        return names;
    }

    public String openSaveDialog() {
        FileChooser fileChooser = new FileChooser();

        //Set extension filter
        fileChooser.getExtensionFilters();

        //Show save file dialog
        File file = fileChooser.showSaveDialog(stage);
        if (file == null)
            return "Cancelled";
        return file.toPath().toString();
    }

    public Boolean setButton(MyButton button, String name) {
        button.setName(name);
        button.setText(name);
        button.setOccupied(true);
        return true;
    }

    public Boolean removeButton(MyButton button) {
        button.setName(null);
        button.setText("Button");
        button.setOccupied(false);
        return true;
    }

    public Boolean storeSelection(String name) {
        if (name == null || name.length() == 0) return false;
        if (!button1.isSet()) return setButton(button1, name);
        if (!button2.isSet()) return setButton(button2, name);
        if (!button3.isSet()) return setButton(button3, name);
        if (!button4.isSet()) return setButton(button4, name);
        if (!button5.isSet()) return setButton(button5, name);
        if (!button6.isSet()) return setButton(button6, name);
        return false;
    }

    public Boolean removeSelection(String name) {
        if (name == null || name.length() == 0) return false;
        if (name.equals(button1.getName())) return removeButton(button1);
        if (name.equals(button2.getName())) return removeButton(button2);
        if (name.equals(button3.getName())) return removeButton(button3);
        if (name.equals(button4.getName())) return removeButton(button4);
        if (name.equals(button5.getName())) return removeButton(button5);
        if (name.equals(button6.getName())) return removeButton(button6);
        return false;
    }

    public void create_buttons() {
        button1 = new MyButton("Button", message_log, listview);
        button2 = new MyButton("Button", message_log, listview);
        button3 = new MyButton("Button", message_log, listview);
        button4 = new MyButton("Button", message_log, listview);
        button5 = new MyButton("Button", message_log, listview);
        button6 = new MyButton("Button", message_log, listview);

        save = new Button("Save");
        save.setMaxWidth(Double.MAX_VALUE);
        save.setMinWidth(Region.USE_PREF_SIZE);

        quit = new Button("Quit");
        quit.setMinWidth(Region.USE_PREF_SIZE);
        quit.setMaxWidth(Double.MAX_VALUE);

        store = new Button("Store in Toolbar");
        remove = new Button("Remove from Toolbar");
        store.setMaxWidth(Double.MAX_VALUE);
        HBox.setHgrow(store, Priority.ALWAYS);
        remove.setMaxWidth(Double.MAX_VALUE);
        HBox.setHgrow(remove, Priority.ALWAYS);

        store.setOnAction(e -> {
            if (storeSelection(listview.getSelectionModel().getSelectedItem())) {
                message_log.appendText(String.format("Added <%s>\n", listview.getSelectionModel().getSelectedItem()));
            } else {
                message_log.appendText(String.format("Failed to add <%s>\n", listview.getSelectionModel().getSelectedItem()));
            }
        });
        remove.setOnAction(e -> {
            if (removeSelection(listview.getSelectionModel().getSelectedItem())) {
                message_log.appendText(String.format("Removed <%s>\n", listview.getSelectionModel().getSelectedItem()));
            } else {
                message_log.appendText(String.format("Failed to remove <%s>\n", listview.getSelectionModel().getSelectedItem()));
            }
        });

        save.setOnAction(e ->
            message_log.appendText(String.format("Save: %s\n", openSaveDialog()))
        );
        quit.setOnAction(e -> System.exit(0));
    }

    public void create_checkboxes() {
        africa = new MyCheckBox("Africa", message_log, country_list, listview);
        asia = new MyCheckBox("Asia", message_log, country_list, listview);
        n_america = new MyCheckBox("North America", message_log, country_list, listview);
        s_america = new MyCheckBox("South America", message_log, country_list, listview);
        europe = new MyCheckBox("Europe", message_log, country_list, listview);
        australia = new MyCheckBox("Australia", message_log, country_list, listview);
    }

    public void create_center() {
        country_label = new Label("Country Name");
        country_label.setFont(new Font("Baskerville", 20));
        country_label.setWrapText(true);

        flag = new ImageView();
        flag.setImage(null);
        description_area = new TextArea();
        description_area.setText("Some Description");
        description_area.setWrapText(true);
    }

    public void create_layout() {
        BorderPane border = new BorderPane();

        HBox topPane = new HBox(10);
        VBox leftPane = new VBox(10);
        GridPane topCenterPane = new GridPane();
        BorderPane centerPane = new BorderPane();
        HBox bottomCenterPane = new HBox(10);
        VBox rightPane = new VBox();
        HBox bottomPane = new HBox();

        GridPane.setColumnIndex(country_label, 0);
        GridPane.setColumnIndex(flag, 1);
        country_label.prefWidthProperty().bind(centerPane.widthProperty());
        centerPane.setBackground(new Background(new BackgroundFill(Color.DARKGREY, CornerRadii.EMPTY, Insets.EMPTY)));
        centerPane.setPadding(new Insets(10, 10, 10, 10));

        VBox vbButtons = new VBox(10);
        vbButtons.setPadding(new Insets(10, 20, 10, 20));
        vbButtons.getChildren().addAll(save, quit);
        vbButtons.setMaxHeight(VBox.USE_PREF_SIZE);

        topPane.setPadding(new Insets(10, 20, 10, 20));
        topPane.getChildren().addAll(button1,button2,button3,button4,button5,button6);
        leftPane.setPadding(new Insets(10, 20, 10, 20));
        leftPane.getChildren().addAll(africa, asia, n_america, s_america, europe, australia);
        topCenterPane.setPadding(new Insets(10, 10, 10, 10));
        topCenterPane.getChildren().addAll(country_label, flag);
        bottomCenterPane.getChildren().addAll(store, remove);
        centerPane.setTop(topCenterPane);
        centerPane.setCenter(description_area);
        centerPane.setBottom(bottomCenterPane);
        rightPane.getChildren().addAll(listview);
        bottomPane.getChildren().addAll(message_log, vbButtons);

        border.setRight(rightPane);
        border.setLeft(leftPane);
        border.setCenter(centerPane);
        border.setTop(topPane);
        border.setBottom(bottomPane);


        Scene scene = new Scene(border, 800, 500);  // Window Size
        stage.setTitle("Assignment 3");    // Window Title
        stage.setScene(scene);
        stage.show();
    }

    public void create_list() {
        country_list  = FXCollections.observableArrayList(getCountryNames());
        sortedList = new SortedList<>(country_list);
        sortedList.setComparator(String::compareToIgnoreCase);

        listview = new ListView<>();
        listview.setItems(sortedList);
        listview.getSelectionModel().select(0);
        Country country = Country.getCountry(listview.getSelectionModel().getSelectedItem());
        if (country != null) {
            country_label.setText(country.Name);
            description_area.setText(country.Description);
            flag.setImage(new Image(String.format("Flags/%s.png", country.Code.toLowerCase())));
        }

        listview.getSelectionModel().selectedItemProperty().addListener((observable, oldValue, newValue) -> {
            Country new_country = Country.getCountry(newValue);
            if (new_country != null) {
                message_log.appendText(String.format("Selected Country: %s\n", new_country.Name));
                country_label.setText(new_country.Name);
                description_area.setText(new_country.Description);
                flag.setImage(new Image(String.format("Flags/%s.png", new_country.Code.toLowerCase())));
            } else {
                message_log.appendText("Selected Country: null\n");
            }
        });
    }

    @Override
    public void start(Stage primaryStage) {

        stage = primaryStage;
        message_log = new TextArea();
        message_log.promptTextProperty().setValue("UI Event Log");
        message_log.setMaxWidth(Double.MAX_VALUE);
        HBox.setHgrow(message_log, Priority.ALWAYS);

        create_center();
        create_list();
        create_buttons();
        create_checkboxes();

        create_layout();
    }


    public static void main(String[] args) {
        launch(args);
    }

}

class MyButton extends Button{

    Boolean occupied;
    String name;
    TextArea message_log;
    ListView<String> listview;
    DropShadow shadow = new DropShadow();

    MyButton(String text, TextArea log, ListView<String> list) {
        super(text);
        occupied = false;
        name = null;
        message_log = log;
        listview = list;

        this.setMaxWidth(Double.MAX_VALUE);
        HBox.setHgrow(this, Priority.ALWAYS);
        this.setOnAction(e -> {
            message_log.appendText(String.format("Selected Button: %s\n", name));
            if (isSet()) {
                listview.getSelectionModel().select(name);
                listview.scrollTo(name);
            } else
                message_log.appendText("Button is currently empty\n");
        });

        //Adding the shadow when the mouse cursor is on
        this.addEventHandler(MouseEvent.MOUSE_ENTERED,
                event -> this.setEffect(shadow));
        //Removing the shadow when the mouse cursor is off
        this.addEventHandler(MouseEvent.MOUSE_EXITED,
                e -> this.setEffect(null));
    }

    public void setOccupied(Boolean occupied){
        this.occupied = occupied;
    }

    public Boolean isSet(){
        return occupied;
    }

    public void setName(String name){
        this.name = name;
    }

    public String getName(){
        return name;
    }


}

class MyCheckBox extends CheckBox {

    TextArea message_log;
    static HashMap<String, LinkedList<String>> continents_list;
    static HashMap<String, Boolean> instances;

    MyCheckBox(String text, TextArea log, ObservableList<String> list, ListView<String> listview) {
        super(text);
        this.setSelected(true);
        this.setMaxHeight(Double.MAX_VALUE);
        VBox.setVgrow(this, Priority.ALWAYS);
        message_log = log;
        if (continents_list == null) {
            create_continents_lists();
            create_hash_map();
        }

        this.setOnAction(e -> {
            String name = this.getText().toLowerCase();
            if (this.isSelected()) {
                message_log.appendText(String.format("Add %s to List\n", name));
                instances.put(name.replace(" ", "_"), true);
            } else {
                message_log.appendText(String.format("Remove %s from List\n", this.getText()));
                instances.put(name.replace(" ", "_"), false);
            }

            list.clear();
            instances.entrySet().stream().filter(Map.Entry::getValue).forEach(entry -> {
                if (entry.getKey().equals("africa")) {list.addAll(continents_list.get("africa"));}
                else if (entry.getKey().equals("asia")) {list.addAll(continents_list.get("asia"));}
                else if (entry.getKey().equals("north_america")) {list.addAll(continents_list.get("north_america"));}
                else if (entry.getKey().equals("south_america")) {list.addAll(continents_list.get("south_america"));}
                else if (entry.getKey().equals("europe")) {list.addAll(continents_list.get("europe"));}
                else if (entry.getKey().equals("australia")) {list.addAll(continents_list.get("australia"));}
            });

            if (listview.getSelectionModel().getSelectedItem() == null) {
                listview.getSelectionModel().select(0);
                message_log.appendText(String.format("Current Selection has been removed, now selected %s\n", listview.getSelectionModel().getSelectedItem()));
            }
        });
    }

    static public void create_hash_map() {
        instances = new HashMap<>();
        instances.put(Country.Continents.AFRICA.toString().toLowerCase(), true);
        instances.put(Country.Continents.ASIA.toString().toLowerCase(), true);
        instances.put(Country.Continents.NORTH_AMERICA.toString().toLowerCase().replace(" ", "_"), true);
        instances.put(Country.Continents.SOUTH_AMERICA.toString().toLowerCase().replace(" ", "_"), true);
        instances.put(Country.Continents.EUROPE.toString().toLowerCase(), true);
        instances.put(Country.Continents.AUSTRALIA.toString().toLowerCase(), true);
    }

    static public void create_continents_lists() {
        LinkedList<String> africa_list = new LinkedList<>();
        LinkedList<String> asia_list = new LinkedList<>();
        LinkedList<String> n_america_list = new LinkedList<>();
        LinkedList<String> s_america_list = new LinkedList<>();
        LinkedList<String> europe_list = new LinkedList<>();
        LinkedList<String> australia_list = new LinkedList<>();
        for (Country country : Country.getCountries()) {
            switch (country.Continent) {
                case AFRICA:
                    africa_list.add(country.Name);
                    break;
                case ASIA:
                    asia_list.add(country.Name);
                    break;
                case NORTH_AMERICA:
                    n_america_list.add(country.Name);
                    break;
                case SOUTH_AMERICA:
                    s_america_list.add(country.Name);
                    break;
                case EUROPE:
                    europe_list.add(country.Name);
                    break;
                case AUSTRALIA:
                    australia_list.add(country.Name);
                    break;
            }
        }

        continents_list = new HashMap<>();
        continents_list.put("africa", africa_list);
        continents_list.put("asia", asia_list);
        continents_list.put("north_america", n_america_list);
        continents_list.put("south_america", s_america_list);
        continents_list.put("europe", europe_list);
        continents_list.put("australia", australia_list);
    }
}
