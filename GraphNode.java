package Assignment4;

import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import static java.lang.Math.max;
import static java.lang.Math.min;


//A simple node class with x and y coordinates and a text value
public class GraphNode {
    //the coordinates and text value are implemented as properties so that the view can listen for events
    private final SimpleDoubleProperty _x = new SimpleDoubleProperty();
    private final SimpleDoubleProperty _y = new SimpleDoubleProperty();
    private final StringProperty text = new SimpleStringProperty("default");
    
    public GraphNode(double X, double Y)
    {
        _x.set(X);
        _y.set(Y);
    }
    
    public double getX()
    {
        return _x.get();
    }
    
    public void setX(double X)
    {
        X = min(X, 1);
        X = max(0, X);
        _x.set(X);
    }
    
    public final DoubleProperty xProperty()
    {
        return _x;
    }
    
    public double getY()
    {
        return _y.get();
    }
    
    public void setY(double Y)
    {
        Y = min(Y, 1);
        Y = max(0, Y);
        _y.set(Y);
    }

    public final DoubleProperty yProperty()
    {
        return _y;
    }
    
    public final StringProperty textProperty() {
        return text;
    }

    public final String getText() {
        return text.get();
    }

    public final void setText(String text) {
        this.text.set(text);
    }
    
    //This will make is so the nodes can easily be displayed in a listview
    @Override
    public String toString()
    {
        return this.text.get();
    }
    
}
