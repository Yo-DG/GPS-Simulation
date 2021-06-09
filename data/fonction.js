var latDepart;
var lonDepart;
var latDestination;
var lonDestination;
var lat;
var lon;
var lonLat;
var zoom;
var markers;
var simu;
var test=0;
var arriveDest=0;

//---------------------------------------------------------------------------------------
function getLat(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("lat").innerHTML = this.responseText;
            document.getElementById("lat2").innerHTML = this.responseText;
            lat=this.responseText; 
        }
    }; 
    xhttp.open("GET", "getLat", true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function getLon(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("lon").innerHTML = this.responseText; 
            document.getElementById("lon2").innerHTML = this.responseText;
            lon=this.responseText;
        }
    }; 
    xhttp.open("GET", "getLon", true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function getDistance(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("d").innerHTML = this.responseText; 
            document.getElementById("d2").innerHTML = this.responseText;
        }
    }; 
    xhttp.open("GET", "getDistance", true);
    xhttp.send();
}
setInterval(function() { 
    getLat();   
    getLon();       
    getDistance();
    getCog();
    arriveDestFct()
}, 1000);

//---------------------------------------------------------------------------------------
function sendData(sim) {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("simState").innerHTML = this.responseText;
            simu=this.responseText;
        }
    };
    xhttp.open("GET", "simu?simState="+sim, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function sendSog(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("sog").innerHTML = this.responseText;
            document.getElementById("sog2").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "sog?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function sendCog(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("cog2").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "cog?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function sendLatDep(val){
    latDepart=val;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("latDep").innerHTML = this.responseText;
            document.getElementById("latDep2").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "latDep?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function sendLonDep(val){
    lonDepart=val;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("lonDep").innerHTML = this.responseText;
            document.getElementById("lonDep2").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lonDep?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function sendLatDest(val){
    latDestination=val;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("latDest").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "latDest?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function sendLonDest(val){
    lonDestination=val;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("lonDest").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "lonDest?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function mapFct(){
    map = new OpenLayers.Map("mapdiv");
    map.addLayer(new OpenLayers.Layer.OSM());

    lonLat = new OpenLayers.LonLat(0,0)
            .transform(
            new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
            map.getProjectionObject() // to Spherical Mercator Projection
            );
                    
    zoom=0;

    markers = new OpenLayers.Layer.Markers( "Markers" );
    map.addLayer(markers);
                
    map.setCenter (lonLat, zoom);
}
mapFct();

//---------------------------------------------------------------------------------------
function mapDep(){
    lonLat = new OpenLayers.LonLat(lonDepart , latDepart)
            .transform(
            new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
            map.getProjectionObject() // to Spherical Mercator Projection
            );
    
    zoom=17;

    markers.addMarker(new OpenLayers.Marker(lonLat));
                
    map.setCenter (lonLat, zoom);
}

//---------------------------------------------------------------------------------------
function mapDest(){
    lonLat = new OpenLayers.LonLat(lonDestination , latDestination)
            .transform(
            new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
            map.getProjectionObject() // to Spherical Mercator Projection
            );
    

    markers.addMarker(new OpenLayers.Marker(lonLat));         
}

//---------------------------------------------------------------------------------------
setInterval(function() { 
    if(simu=="ON"){          
        lonLat = new OpenLayers.LonLat(lon , lat)
            .transform(
            new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
            map.getProjectionObject() // to Spherical Mercator Projection
            );
        markers.addMarker(new OpenLayers.Marker(lonLat));
    }
}, 1000);

//---------------------------------------------------------------------------------------
function displayDest(){
    choixDest.style.display = "block";
    choix.style.display = "none";
}

//---------------------------------------------------------------------------------------
function displayDataDest(){
    dataDest.style.display = "block";
    choixDest.style.display = "none";
    markers.clearMarkers();
    sendSog(document.getElementById('sogV').value);
    sendLatDep(document.getElementById('latDepV').value);
    sendLonDep(document.getElementById('lonDepV').value);
    sendLatDest(document.getElementById('latDestV').value);
    sendLonDest(document.getElementById('lonDestV').value);
    paramSim(1);
    destOrCog(1);
    sendData(1);
    mapDep();
    mapDest();
}

//---------------------------------------------------------------------------------------
function displayCog(){
    choixCog.style.display = "block";
    choix.style.display = "none";
}

//---------------------------------------------------------------------------------------
function displayDataCog(){
    dataCog.style.display = "block";
    choixCog.style.display = "none";
    markers.clearMarkers();
    sendSog(document.getElementById('sogV2').value);
    sendCog(document.getElementById('cogV').value);
    sendLatDep(document.getElementById('latDepV2').value);
    sendLonDep(document.getElementById('lonDepV2').value);
    paramSim(1);
    destOrCog(0);
    sendData(1);
    mapDep();
}

//---------------------------------------------------------------------------------------
function displayChoix(){
    dataCog.style.display = "none";
    dataDest.style.display = "none";
    choix.style.display = "block";
    sendData(0);
    if(arriveDest==0){
        arriveDestination.style.display = "none";
    }
    markers.clearMarkers();
}

//---------------------------------------------------------------------------------------
function paramSim(val){
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "paramSimu?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function destOrCog(val){
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "destOrCog?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function getCog(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("cog").innerHTML = this.responseText; 
        }
    }; 
    xhttp.open("GET", "getCog", true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function arriveDestFct(){
    var xhttp = new XMLHttpRequest();
    
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            arriveDest = this.responseText; 
        }
    }; 
    xhttp.open("GET", "arriveDest", true);
    xhttp.send();
    if(arriveDest==1){
        arriveDestination.style.display = "block";
        sendData(0);
    }
}