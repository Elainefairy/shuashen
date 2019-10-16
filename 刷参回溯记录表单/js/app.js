$(function() {

	var addBtnClicked = false;

	var url = "/etable/data.php?action=init_data_list"
	$.get(url,function(data){
		d = eval('(' + data + ')');
		for(m in d ){
			var rowObj = createRow(d[m]);
			$("table.data").append(rowObj)
		}
	});

	$("#addBtn").click(addBlankRow);

	function addBlankRow(){
		if(false == addBtnClicked){
			addBtnClicked = true;
			newrow = $("<tr></tr>");
			for(var i=0; i < 8 ; i++){
				newrow.append($("<td><input class='txtField' type='text' value='' " + "/></td>"));
			}
	
			saveButton = $("<a class='optLink' href='javascript:;'></a>");
			saveButton.html("Save");
	
	
			cancelButton = $("<a class='optLink' href='javascript:;'>");
			cancelButton.html("Cancel&nbsp;");
	
			cancelButton.click(function(){
				addBtnClicked = false;	
				newrow.remove();
			});
	
			saveButton.click(function(){
				values = $(newrow).find("input[type=text]");
				postData = {};
				$(values).each(function(i,obj){
					f = "c_" + i;
					postData[f] = $(obj).val();
				});
				$.post("/etable/data.php?action=add_row",postData,function(res){
					if(0 < res)	{
						addrowObj = postData;	
						postData["id"] = res;
						newrow.remove();
						$("table.data").append(createRow(addrowObj));
						addBtnClicked = false;
					} else {
						alert("保存失败.");	
					}
				});
			});
	
			var optCol = $("<td></td>")
			
			optCol.append(cancelButton);
			optCol.append(saveButton);
			newrow.append(optCol);
	
			$("table.data").append(newrow);
		}
	}

	function createRow(rowObj){
		rowObj = rowObj;
		var rowId;
		rowDom = $("<tr></tr>");
		for(m in rowObj){
			if(m=="id"){
				rowId = rowObj[m];
			}else{
				rowDom.append($("<td>" + rowObj[m] +"</td>"));
			}
		}	
	
		delButton = $("<a href='javascript:;' class='optLink'></a>");
		delButton.html("Del&nbsp;");
		delButton.data("currentId",rowId);
		delButton.click(delRowHandler);
	
		editButton = $("<a href='javascript:;' class='optLink'></a>");
		editButton.html("Edit ");
		editButton.data("currentId",rowId);
		editButton.click(editRowHandler)
	
		optCol = $("<td></td>");
		optCol.append(delButton);
		optCol.append(editButton);
	
		rowDom.append(optCol);
		return rowDom;
	}

	function delRowHandler(e){
		delButton = e.target;
		rowId = $(delButton).data("currentId");
		if(window.confirm("真的要删除吗?")){
			$.post("/etable/data.php?action=del_row",{delId:rowId},function(res){
				if(res == "OK")	{
					$(delButton).parent().parent().remove();
				} else {
					alert("删除失败.");
				}
			});
		}
	}

	function editRowHandler(e){
		var editButton = e.target;
		var rowId = $(editButton).data("currentId");
		var editRow = $(editButton).parent().parent();
		var tdValues = $(editRow).find("td");
		var editRowHtml = $(editRow).html();
		$(tdValues).each(function(i,obj){
			if(i == 8){
	
				$(obj).html("");
	
				var saveButton = $("<a class='optLink' href='javascript:;'></a>");
				saveButton.html("Save ");
	
				var cancelButton = $("<a class='optLink' href='javascript:;'></a>");
				cancelButton.html("Cancel");
	
				cancelButton.click(function(){
					$(editRow).html(editRowHtml);
					$(editRow).find("a").eq(0).click(delRowHandler);
					$(editRow).find("a").eq(1).click(editRowHandler);
					$(editRow).find("a").data("currentId",rowId);
				});
	
				saveButton.click(function(){
					values = $(editRow).find("input[type=text]");
					postdata = {};
					$(values).each(function(i,obj){
						f = "c_" + i;
						postdata[f] = $(obj).val();
					});
					postdata["id"] = rowId;
					$.post("/etable/data.php?action=edit_row",postdata,function(res){
						if("OK" == res)	{
							var updaterow = createRow(postdata);
							editRow.replaceWith(updaterow);
						} else {
							alert("保存失败.");	
						}
					});
				});
	
				$(obj).append(saveButton);
				$(obj).append(cancelButton);
	
			} else {
				$(obj).html("<input class='txtField' type='text' value='"+$(obj).html()+"'/>");
			}
		});
	}

});
