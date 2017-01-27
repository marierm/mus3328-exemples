import http.requests.*;

public void setup() 
{
  size(400,400);
  smooth();
  
  GetRequest get = new GetRequest("http://earthquake.usgs.gov/earthquakes/feed/v1.0/summary/all_hour.geojson");
  get.send(); // program will wait untill the request is completed
  //println("response: " + get.getContent());

  JSONObject response = parseJSONObject(get.getContent());
  //println("status: " + response.getString("status"));
  JSONArray boxes = response.getJSONArray("features");
  //println("boxes: ");
  for(int i=0;i<boxes.size();i++) {
    JSONObject box = boxes.getJSONObject(i);
    JSONObject goat = box.getJSONObject("properties");
    println("  mag: " + goat.getInt("mag"));
    println("  place: " + goat.getString("place"));
  }
}
