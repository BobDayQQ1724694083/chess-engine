function alert1(){
	$('#editor_alert').empty();
	var n = $("#editor_alert").noty({
		text : 'Please click View Full Problem to enable code editor.',
		type : 'error',
		timeout : 2000
	});
	
	
//alert("Please click View Full Problem to enable code editor.");
}

function checkFileSize(inputFile,compilationstatus) {
    var max =  100 * 1024; // 100kB
   
    var status = compilationstatus;
    if(status == "STOPPED")
    	{
    		alert("Evaluation service is not started. Please submit after some time");
    		inputFile.value = null;  // Clear the field.
    		return false;
    	}
    else if(status == "STARTED"){
    if (inputFile.files && inputFile.files[0].size > max) {
        alert("File too large. Maximum file size can be 100KB"); 
        inputFile.value = null; // Clear the field.
        return false;
    }else if(!checkspec(inputFile)){
    	 alert ("Filename should not contain special characters"); 
    	inputFile.value = null;
    	 return false;
    }
    else
    	return true;
	}
}

function checkspec(inputFile){
	
	var splChars = "*|,\"<>[]{}`\';()@&$#%";
	var i =0;
	for (; i < inputFile.value.length; i++) {
	    if (splChars.indexOf(inputFile.value.charAt(i)) != -1){
	   
	    break;
	}
	}
	    if(i == inputFile.value.length){
	    return true;	
	    }
	    else{
	    	return false;
	    	}
	    
}
function checkStatus() {  
		console.log("in checkstatus making url...");
		console.log("selected problem is"+ $('input[name=selected_problem]').val());
			
		       //location.reload();
		//	my_window = window.open("editor.jsp?problemid="+$('input[name=selected_problem]').val(), "mywindow1", "scrollbars=1,status=1,width=1350,height=650,resizable=false, toolbar=no, menubar=no, location=no"); 
			//mywindow1.document.title = 'Hello, world!';
			
			my_window = window.open("execute.jsp?problemid="+$('input[name=selected_problem]').val(), "mywindow1", "scrollbars=1,status=1,width=1000,height=665,top=0,left=0,resizable=false,toolbar=no, menubar=no");
		}
	
function select_file(id,input_file,compilationstatus) {
	
	var actual_file = $('#'+id);
	if(!(checkFileSize(input_file,compilationstatus)))
		return false;
	else{
	var fake_file = actual_file.next().find('input[name=fake_file_input]');
    fake_file.val(actual_file.val());
	}
}


		$('input.confirm').on('click', function(){
			if($(this).val() == 1){
				$(this).parent().find('.links-comments').hide();
				$(this).parent().find('input.confirm').prop("checked",false);
				$(this).parent().find('input.confirm.default-confirm').prop("checked",true);
			}else if($(this).val() == 0){
				$(this).parent().find('.links-comments').show();
				$(this).parent().find('input.confirm').prop("checked",true);
				$(this).parent().find('input.confirm.default-confirm').prop("checked",false);
			}
		});

		
		$('input[name=Submit]').click(function(e){  
			
			var fileElement = $(this).parent().prev().find('input[type=file]');
			var uploadedFile = fileElement.val();
			var id = fileElement.attr("id");
			var dLang = '${defaultLang}';
			var selectedExtension;
			var extensionLang ;
			if(id === "Program_A"){
				selectedExtension = $('#extensionA').val();	
			}else if(id === "Program_B"){
					selectedExtension = $('#extensionB').val();	
			}else if(id === "Program_C"){
				selectedExtension = $('#extensionC').val();
			}else if(id === "Program_D"){
				selectedExtension = $('#extensionD').val();
			}else if(id === "Program_E"){
				selectedExtension = $('#extensionE').val();	
			}else if(id ===  "Program_F"){
				selectedExtension = $('#extensionF').val();	
			}else if(id ===  "Program_G"){
				selectedExtension = $('#extensionG').val();	
			}else if(id ===  "Program_H"){
				selectedExtension = $('#extensionH').val();	
			}else if(id ===  "Program_I"){
				selectedExtension = $('#extensionI').val();	
			}else if(id ===  "Program_J"){
				selectedExtension = $('#extensionJ').val();	
			}
			
			extensionLang = selectedExtension;
			
			if(selectedExtension == ".java7" || selectedExtension == ".java8"){
				selectedExtension = ".java";
			}
			
			if(selectedExtension == ".py2" || selectedExtension == ".py3"){
				selectedExtension = ".py";
			}
			
			if(uploadedFile.length>45)
			{
				alert ("file name should be less than 45 characters");
				return false;
			}
			var extension = uploadedFile.split(".");
			if (uploadedFile == null || uploadedFile == "") {
				alert("Please upload your answer");
				return false;
			}			
			else if(checkExtension(extension)==false){
				return false;
			}else if(extension.length>2){
				alert("Valid File format programName.extension");
				return false;
			}else if(selectedExtension == null || selectedExtension == ""){
				alert("Please select language");
				return false;
			}else if(selectedExtension != "."+extension[1]){
					alert("Selected language and file extension are not matching");
					return false;
			}else {
				
				// Select One of the option below Yes or No
				
				
				var confirmed = $(this).parent().parent().find('input.confirm:checked')[0].value;
				console.log("confirmed selected is "+confirmed);
				var responseDiv = $(this).next();
				var done = true;
				var links = "";
				var comments = "";
				if(confirmed === "0"){
					links = $(this).parent().parent().find('textarea.text-links').val();
					comments = $(this).parent().parent().find('textarea.text-comments').val();
					if(links == "" || comments == ""){
						alert("Mention links and comments");
						done = false;
					}
				}
				
				console.log("links are: "+links);
				console.log("comments are : "+comments);
				if(done){
					 responseDiv.html('<img id="loading" src="/CodevitaV6/images/pp.gif" height="10px" style="margin-left:5px;">');
					 $.ajaxFileUpload({
		                url:'/CodevitaV6/subServlet',
		                secureuri:false,
		                fileElementId: fileElement.attr("id"),
		                dataType: 'json',
		                data :{radio:$('input[name=selected_problem]').val(), extension:extensionLang, confirmed:confirmed, links:links, comments:comments},
		                success: function (data, status){
		                    if(typeof(data.error) != 'undefined') {
		                        if(data.error != '') {
		                        	if(data.error == 'session_expired') {	
		                        		responseDiv.html('<img src="/CodevitaV6/images/error.png" style="margin-left:5px;vertical-align: middle;" width="18px">Session Expired');
		                        		alert('Your session has expired, please login and try again.');
										window.open("/CodevitaV6/logout.jsp", "_self");
		                        	}else if(data.error == 'Your contest has ended'){
		                        		responseDiv.html('<img src="/CodevitaV6/images/error.png" style="margin-left:5px;vertical-align: middle;" width="18px">Your contest has ended');
		                        		alert('Your contest has ended. Thank you for your participation.');
										window.open("/CodevitaV6/guidelines.jsp", "_self");
		                        	}else {
			                        	responseDiv.html('<img src="/CodevitaV6/images/error.png" style="margin-left:5px;vertical-align: middle;" width="18px">' + data.error);		                        		
		                        	}
		                        } else {
		                        	/*setTimeout( function() {
											responseDiv.html('<%@ include file="ajaxresult.jsp"%>' + data.msg);
											}, 4000);			
											setTimeout( function() {
											responseDiv.html('<img src="/CodevitaV6/images/TickMark.gif" style="margin-left:5px;vertical-align: middle;">' + data.msg);
											alert("95");
											}, 4000);*/					
		                        			responseDiv.html('<img src="/CodevitaV6/images/TickMark.gif" style="margin-left:5px;vertical-align: middle;">' + data.msg +'.<br>Please check    <a href="/CodevitaV6/ChkSubmissionServlet" style="color:blue">My Submissions</a> tab.');	
		                        			//alert("98");
											//setTimeout( function() {getContent('/CodevitaV6/ajaxresult.jsp?problemid='+$('input[name=selected_problem]').val(),responseDiv);},2000);
											//getContent('/CodevitaV6/ajaxresult.jsp',responseDiv);
											/*setTimeout( function() {
											responseDiv.html(''+ data.msg);
											}, 4000);	*/	  
		                        }
		                        
		                        $('input.confirm').prop("checked",false);
		                        $('input.confirm.default-confirm').prop("checked",true);
		                        $('.links-comments').hide();
		                        $('textarea.text-links').val('');
		                        $('textarea.text-comments').val('');
	                        	$('input[type=file]').val('');
	                        	$('input[name=fake_file_input]').val('');
	                        	if(dLang){
	                        		$("select[name='language']").val(dLang);
	                        	}else{
	                        		$('select[name=extension]').val('');
	                        	}
		                    }
		                },
		                error: function (data, status, e){
		                	if(e=='SyntaxError: Unexpected token {')
		                	{responseDiv.html('<img src="/CodevitaV6/images/TickMark.gif" style="margin-left:5px;vertical-align: middle;">Evaluation in progress.<br> Please check <a href="/CodevitaV6/ChkSubmissionServlet" style="color:blue">My Submissions</a> tab after some time.');}
//		                	else if(e =='SyntaxError: Unexpected token <'){
//		                		responseDiv.html('<img src="/CodevitaV6/images/error.png" style="margin-left:5px;vertical-align: middle;" width="18px">' +  data.msg);
//		                	
//		                		}
		                	
		                		
		                		else{
		                	responseDiv.html('<img src="/CodevitaV6/images/error.png" style="margin-left:5px;vertical-align: middle;" width="18px">' +  e+'hi');
		                	$('input.confirm').prop("checked",false);
	                        $('input.confirm.default-confirm').prop("checked",true);
		                	$('.links-comments').hide();
		                	$('textarea.text-links').val('');
		                    $('textarea.text-comments').val('');
                       	 	$('input[type=file]').val('');
                       	 	$('input[name=fake_file_input]').val('');
	                       	 if(dLang){
	                     		$("select[name='language']").val(dLang);
	                     	}else{
	                     		$('select[name=extension]').val('');
	                     	}
                       	 	}
		                }
		            });
				// End of sending code here }
				}
			}
			return true;
		});
		function getContent(filename,responseDiv)
                    {
                        $.ajax({
                            url: filename,
                            type: 'GET',
                            dataType: 'html',
                            beforeSend: function() {
                                responseDiv.html('<img id="loading" src="/CodevitaV6/images/pp.gif" height="10px" style="margin-left:5px;">');
                            },
                            success: function(data, textStatus, xhr) {

                                  if (filename == '/CodevitaV6/ajaxresult.jsp?problemid='+$('input[name=selected_problem]').val())
                                {
                                    setTimeout( function() {
                                        responseDiv.html(data);
                                    }, 2000);
									responseDiv.html(data);
                                }
                            }
                        });
                    }
	
		$(document).ready(function() {		
			selectProblem();
			if(typeof(EventSource) !== "undefined") {			
				//var notification_server = '${notification_server}';
				var notification_server ='ws://54.225.248.164:49182';
				var ws_problemstart;
				try {
					ws_problemstart = new WebSocket(notification_server);	
				} catch(e) {
					$('#notification_list').parent().html('<div id="compatibility_info">' +
							'<p id="compatibility_info_heading">' +
								'Oops! your browser does not support notifications' +
								'</p>' +
								'<font id="compatibility_info_suggestion">' +
									'Suggestions:' + 
								'</font>' +
								'<ul id="compatibility_info_suggestion_ul">' +						
									'<li>' +
										'Consider using different browser e.g.' + 
										'<a href="http://www.mozilla.org/en-US/firefox/new/" target="_blank"><img src="/CodevitaV6/images/firefox_icon.gif" style="width: 15px; vertical-align: middle; border: medium none;"></a> or ' + 
										'<a href="https://www.google.com/intl/en/chrome/browser/" target="_blank"><img src="/CodevitaV6/images/chrome_icon.gif" style="width: 15px; vertical-align: middle; border: medium none;"></a>' +
									'</li>' +			  												
								'</ul>' +
								'</div>');
				} 
				ws_problemstart.onopen = function() {
					console.log('Connection opened');	
					ws_problemstart.send('${uid}' + ':<%= contestid %>');	
				};
	
				ws_problemstart.onmessage = function(e) {						
					var not_data = e.data.split("#");					
					var not_inner_html = not_data[0] + ' at ' + not_data[1];
					var event_name = not_data[2];
					
					$("#notification_popup_box").stop(true, true);	
					
					$("#notification_popup_text").html(not_inner_html);
					if (event_name.toLowerCase() =='accepted') {
						$("#notification_popup_box").removeClass('notification_popup_box_amber');
						$("#notification_popup_box").removeClass('notification_popup_box_yellow');
						$("#notification_popup_box").addClass('notification_popup_box_green');
						
						$("#close_icon").removeClass('amberIcon');
						$("#close_icon").removeClass('yellowIcon');
						$("#close_icon").addClass('greenIcon');
						
					} else if (event_name.toLowerCase() =='time_exceeded') {
						$("#notification_popup_box").removeClass('notification_popup_box_green');
						$("#notification_popup_box").removeClass('notification_popup_box_yellow');
						$("#notification_popup_box").addClass('notification_popup_box_amber');
						
						$("#close_icon").removeClass('greenIcon');
						$("#close_icon").removeClass('yellowIcon');
						$("#close_icon").addClass('amberIcon');
						
					} else if (event_name.toLowerCase() =='min_solution_time') {
						$("#notification_popup_box").removeClass('notification_popup_box_amber');
						$("#notification_popup_box").addClass('notification_popup_box_green');
						$("#notification_popup_box").addClass('notification_popup_box_yellow');
						
						$("#close_icon").removeClass('amberIcon');
						$("#close_icon").removeClass('amberIcon');
						$("#close_icon").addClass('yellowIcon');
					}
									
					$("#notification_popup_box").show("slide", { direction: "right"}, 200).delay(10000).hide("slide", { direction: "right"}, 200);
					
					$('#notification_list').html('<li>' + not_inner_html + '</li>' + $('#notification_list').html());
					console.log('Message from server: ', e.data);
				};
	
				ws_problemstart.onclose = function() {
					console.log('Connection closed');
				}; 
	
				$(window).bind('beforeunload',function(){
					ws_problemstart.close();
				});
	
				$(window).unload(function() {
					ws_problemstart.close();
				});
		  	} else {
		  		$('#notification_list').parent().html('<div id="compatibility_info">' +
						'<p id="compatibility_info_heading">' +
							'Oops! your browser does not support notifications' +
							'</p>' +
							'<font id="compatibility_info_suggestion">' +
								'Suggestions:' + 
							'</font>' +
							'<ul id="compatibility_info_suggestion_ul">' +						
								'<li>' +
									'Consider using different browser e.g.' + 
									'<a href="http://www.mozilla.org/en-US/firefox/new/" target="_blank"><img src="/CodevitaV6/images/firefox_icon.gif" style="width: 15px; vertical-align: middle; border: medium none;"></a> or ' + 
									'<a href="https://www.google.com/intl/en/chrome/browser/" target="_blank"><img src="/CodevitaV6/images/chrome_icon.gif" style="width: 15px; vertical-align: middle; border: medium none;"></a>' +
								'</li>' +			  												
							'</ul>' +
							'</div>');
		  	}
		}); 
				
		function closeNotification() {
			$("#notification_popup_box").stop(true, true);
			$("#notification_popup_box").hide();
			$("#notification_popup_text").html('');
		} 
		
		$("#tabs").on( "tabsactivate", function( event, ui ) {
			console.log("running selectProblem");
			selectProblem();
		} );
		
		function selectProblem() {
			var tab_index = $("#tabs").tabs( "option", "active" );
			console.log("tab_index "+tab_index);
			switch(tab_index) {
				case 0: 	$('input[name=selected_problem]').val('A');	
							//setFileWidth('A');
							set_notification_height(1);
			  				break;
			 	case 1:		$('input[name=selected_problem]').val('B');	
						 	//setFileWidth('B');			
						 	set_notification_height(2);
	 						break;
			 	case 2:		$('input[name=selected_problem]').val('C');	
			 				//setFileWidth('C');
			 				set_notification_height(3);
						 	break;
			 	case 3:		$('input[name=selected_problem]').val('D');	
			 				//setFileWidth('D');
			 				set_notification_height(4);
						 	break;
			 	case 4:		$('input[name=selected_problem]').val('E'); 
			 				//setFileWidth('E');
			 				set_notification_height(5);
						 	break;
			 	case 5:		$('input[name=selected_problem]').val('F'); 
			 				//setFileWidth('F');
			 				set_notification_height(6);
						 	break;
				case 6:		$('input[name=selected_problem]').val('G'); 
			 				//setFileWidth('F');
			 				set_notification_height(7);
						 	break;
				case 7:		$('input[name=selected_problem]').val('H'); 
			 				//setFileWidth('F');
			 				set_notification_height(8);
						 	break;
				case 8:		$('input[name=selected_problem]').val('I'); 
 							//setFileWidth('F');
 							set_notification_height(9);
 							break;
				case 9:		$('input[name=selected_problem]').val('J'); 
							//setFileWidth('F');
							set_notification_height(10);
							break;
			
			}
		}		
		
		function setFileWidth(id) {
			$('#Program_' + id).width($('#Program_' + id).next().find('input[name=Browse]').width() + $('#Program_' + id).next().find('input[name=fake_file_input]').width() + 30);
			$('.fileinputs').width($('#Program_' + id).width());
			$('.fakefile').width($('#Program_' + id).width());
			$('.filesubmit').css('margin-left', $('#Program_' + id).width());
		}
		
		function set_notification_height(index) {
			var a = $('#tabs-' + index).find('div[class=border]').height();
			var b = $('#notification_div').offset().top;
			var c = $('.timenotif').parent().offset().top;
			if ((a - (b-c) + 22) > 150) {
				//if ($('#notification_list').height() > $('#notification_div').height())
				$('#notification_div').height(a - (b-c) + 22);
			} else {
				$('#notification_div').height(150);
			}		
		}
		
		/*function checkExtension(extension){
			if(($.inArray("cpp", extension) <=-1)	&& ($.inArray("c", extension) <=-1) 
						){
				alert("Programming languages supported are \n\n			C - .c\nC++ - .cpp");
				return false;
			}
		}*/
		function checkExtension(extension){
			if(($.inArray("java", extension) <= -1) && 
						($.inArray("c", extension) <=-1) 	&& 
						($.inArray("cpp", extension) <=-1)	&&
						($.inArray("pl", extension) <=-1)	&& 
						($.inArray("rb", extension) <=-1)	&& 
						($.inArray("py", extension)	<=-1)	&& 
						($.inArray("cs", extension) <=-1)   && 
						($.inArray("php", extension) <=-1)  && 
						($.inArray("js", extension)	<=-1)   && 
						($.inArray("jl", extension)	<=-1) )  {
				alert("Programming languages supported are \n\n			C# - .cs,\n			C++ - .cpp,\n			C - .c,\n			Java - .java,\n			Perl - .pl,\n			Python - .py,\n			Ruby - .rb,\n			Php - .php,\n			Javascript - .js,\n			Julia - .jl");
				return false;
			}
		}
