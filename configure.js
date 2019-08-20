const fs = require("fs");
const path = require("path");

/*
用于提示选择配置目录

选择config下的目录，选择完成后产生config/selected.txt文件

*/

const save_file_ = "config/selected.txt";
const setting_in_file_ = "config/settings.json.in";
const setting_file_ = ".vscode/settings.json";

var current_select_ = "config";

var current_dirs_ = [];

var finish_ = false;



//选择结果处理
function finishSelect(sel){
	console.log("Selected Config: " + current_select_);
	fs.writeFileSync(save_file_, current_select_);	
	
	try{
		var dir = path.posix.normalize(sel);
		var dir_win32 = path.win32.normalize(sel);
		dir_win32 = dir_win32.replace("\\","\\\\");
		var data = fs.readFileSync(setting_in_file_);
		var str = data.toString();
		str = str.replace("${CONFIG_PATH}", dir);
		str = str.replace("${CONFIG_PATH_WIN32}", dir_win32);
		fs.writeFileSync(setting_file_, str);
		
	}catch(e){
		console.log(e);
	}	
	process.exit(0);
}


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
		finishSelect(current_select_);

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





