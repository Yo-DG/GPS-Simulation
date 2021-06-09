var arriveDest=0;

//---------------------------------------------------------------------------------------
function getLat(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("lat").innerHTML = this.responseText; 
            document.getElementById("lat2").innerHTML = this.responseText; 
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
function wifiConfig(){
    ssid(document.getElementById('ssidV').value);
    password(document.getElementById('passwordV').value);
    ip(document.getElementById('ipV').value);
    dns(document.getElementById('dnsV').value);
    gateway(document.getElementById('gatewayV').value);
    subnet(document.getElementById('subnetV').value);
}

//---------------------------------------------------------------------------------------
function ssid(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("ssid").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "ssid?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function password(val){
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "password?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function ip(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("ip").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "ip?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function dns(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("dns").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "dns?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function gateway(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("gateway").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "gateway?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function subnet(val){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            document.getElementById("subnet").innerHTML = this.responseText;
        }
    };
    xhttp.open("GET", "subnet?value="+val, true);
    xhttp.send();
}

//---------------------------------------------------------------------------------------
function displayWf(){
    ligneWf1.style.display = "block";
    ligneWf2.style.display = "block";
    btnWf.style.display ="inline-block";
    btnDisplayWf.style.display = "none";
}

//---------------------------------------------------------------------------------------
function displayDest(){
    choixDest.style.display = "block";
    choix.style.display = "none";
}

//---------------------------------------------------------------------------------------
function displayDataDest(){
    dataDest.style.display = "block";
    choixDest.style.display = "none";
    sendSog(document.getElementById('sogV').value);
    sendLatDep(document.getElementById('latDepV').value);
    sendLonDep(document.getElementById('lonDepV').value);
    sendLatDest(document.getElementById('latDestV').value);
    sendLonDest(document.getElementById('lonDestV').value);
    paramSim(1);
    destOrCog(1);
    sendData(1);
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
    sendSog(document.getElementById('sogV2').value);
    sendCog(document.getElementById('cogV').value);
    sendLatDep(document.getElementById('latDepV2').value);
    sendLonDep(document.getElementById('lonDepV2').value);
    paramSim(1);
    destOrCog(0);
    sendData(1);
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
}

//---------------------------------------------------------------------------------------
function paramSim(val){
    var xhttp = new XMLHttpRequest();
    xhttp.open("GET", "paramSimu?value="+val, true);
    xhttp.send();
}

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