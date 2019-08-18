const fs = require("fs");

/*
用于提示选择配置目录

选择config下的目录，选择完成后产生config/selected.txt文件

*/

const save_file_ = "config/selected.txt";

var current_select_ = "config";

var current_dirs_ = [];

var finish_ = false;

//显示输入提示
function displaySelect(arr, reselect){
	if(!reselect){
		console.log("------------------------------------------");
		for(var i=0;i<arr.length;i++){
			console.log("[" + (i + 1) + "] " + arr[i]);
		}
	}
	console.log("Please select config by input number [1-" + arr.length + "]");
}

//选择指定目录
function selectDir(dir){
	var subfiles = fs.readdirSync(dir);
	var subdirs = [];
	for(var i in subfiles){
		if(fs.statSync(dir + "/" + subfiles[i]).isDirectory()){
			subdirs.push(subfiles[i]);
		}
	}
	if(subdirs.length){
		if(subdirs.length > 1){			
			current_dirs_ = subdirs;
			displaySelect(current_dirs_);
		}else{
			current_select_ += ("/" + subdirs[0]);
			selectDir(current_select_);
		}
	}else{	
		finish_ = true;
		console.log("------------------------------------------");
		current_select_ = current_select_.substr(7); //remove "config/"
		console.log("Config: " + current_select_);
		fs.writeFileSync(save_file_, current_select_);
		//var cmakestr = "include(${CMAKE_CURRENT_LIST_DIR}/" + current_select_ + " OPTIONAL)";
		//fs.writeFileSync(cmake_file_, cmakestr);
		process.exit(0);
	}
}

//处理stdin输入
process.stdin.on('data', function(data){
	var selectok = false;
	try{
		var sel = parseInt(data.toString()); 
		if(sel>0 && sel <=current_dirs_.length){
			var seldir = current_dirs_[sel - 1];
			console.log("Select: " + seldir);
			current_select_ = current_select_ + "/" + seldir
			selectDir(current_select_);
			return;
		}
	}catch(e){
		
	}
	if(finish_){
		process.exit(0);
	}
	if(!selectok){
		displaySelect(current_dirs_, true);
	}
});

selectDir(current_select_);
