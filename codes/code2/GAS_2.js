var sheet_id = "XXX填入google sheet網址.../d/到/edit...這段的內容"; 
var sheet_name = "工作表名稱（下面那個工作表名稱）"; 

function doGet(e){ 
  var ss = SpreadsheetApp.openById(sheet_id); 
  var sheet = ss.getSheetByName(sheet_name); 
  
  var weight = Number(e.parameter.weight); 
  var num = Number(e.parameter.num)
  var num_A = Number(e.parameter.num_A)
  var num_B = Number(e.parameter.num_B)
  
  var date = new Date()
  var current_month = Number(date.getMonth()+1);
  var current_day = Number(date.getDate());
  var current_hour = Number(date.getHours());
  var current_minute = Number(date.getMinutes());

  sheet.appendRow([current_month, current_day, current_hour, current_minute, weight, num, num_A, num_B]); 
}