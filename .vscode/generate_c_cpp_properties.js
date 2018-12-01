const fs = require("fs");
const child_process = require("child_process");
const os = require('os');


var cc = "g++";
var args = ["-c","-v","-E","-"];

var startSign = "<...>"

var includes = [];

var opt = {};
opt.input = "\n";

var str = "";

try{
	
//exec: g++ -c -v -E -	
var obj = child_process.spawnSync(cc,args, opt);
str = obj.stderr.toString();
var lines = str.split("\n");

//find include from output
var inHeader = false;
for(var i in lines){
	var line = lines[i];
	if(inHeader){
		var ch1 = line.charAt(0);
		if(ch1 == ' '){
			includes.push(line.trim() + "/**");
		}else{
			break;
		}
	}else{
		if(line.indexOf(startSign)>0){
			inHeader = true;
		}
	}
}

}catch(e){
	console.log(e);
}

//print 
console.log("include directories:");
for(var i in includes){
	var include = includes[i];
	console.log(include);
}

//load old properties
var jsonfile = "c_cpp_properties.json";
var props = null;
try{
	var data = fs.readFileSync(jsonfile);
	props = JSON.parse(data.toString());
}catch(e){
	
}
if(!props){
	props = {};	
}
if(!props.configurations){
	props.configurations = [];
}

//get configName
var configName = "";

var platform = os.platform();
if(platform == "win32"){
	configName = "Win32";
}else if(platform == "darwin"){
	configName = "Mac";
}else{
	configName = "Linux";
}

//get config
var config = null;
for(var i in props.configurations){
	var conf = props.configurations[i];
	if(conf.name == configName){
		config = conf;
		break;
	}
}

//merger includes to configurations
var changed = false;
if(!config){ //not found
	var newconf = {};
	newconf.name = configName;
	newconf.includePath = includes;
	newconf.includePath.push("${workspaceFolder}/**");
    newconf.cStandard="c11";
    newconf.cppStandard= "c++11";	
	newconf.intelliSenseMode="gcc-x64";
	newconf.defines = [];
	//newconf.compilerPath="g++";
	props.configurations.push(newconf);
	if(!props.version)props.version = 4;
	changed = true;
}else{ //found
	if(!config.includePath)config.includePath = [];
	for(var i in includes){
		var inc = includes[i];
		if(config.includePath.indexOf(inc) < 0){
			changed = true;
			config.includePath.push(inc);
		}
	}
}

//save configurations

if(changed){
	var data = JSON.stringify(props,null,4);
	//console.log(data);
	fs.writeFileSync(jsonfile, data);
	console.log("saved to " + jsonfile);
}else{
	console.log("not changed");
}

