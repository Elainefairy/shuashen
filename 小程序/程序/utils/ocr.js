
var app

var __getBaiduToken = function (options) {
  var tokenCache

  options = { 
    success: options && options.success ? options.success : function () { }, 
    fail: options && options.fail ? options.fail : function () { }
  }

  //取缓存token
  /*try {
    tokenCache = wx.getStorageSync('baidu_ocr_token')
    if (tokenCache){
      // 单位： s
      var tokenTime = (new Date().getTime() - tokenCache.inTime)/1000
      // 提前token过期一小时前更新token防止临界点
      if (tokenTime > tokenCache.expires_in - 3600){
        tokenCache = null
      }
    }
  } catch (e) {
    console.log(e)
  }*/

  /*if (tokenCache) {
    app.baiduOcrToken = tokenCache.access_token
    options.success(app.baiduOcrToken)
    return
  }*/
 
  //缓存token没有，重新获取
  wx.request({
    url: 'https://ecnu-iot.emlab.net' ,
    header: {
      // 'content-type': 'application/json'
      'Content-Type': 'json'
    },
    dataType: "json",
    success(result) {
      console.log('获取token成功')
      if (result.data.access_token){
        result.data.inTime = new Date().getTime()
        //wx.setStorageSync('baidu_ocr_token', result.data)
        //app.baiduOcrToken = result.data.access_token
        //options.success(app.baiduOcrToken)
      }else{
        options.fail()
        console.log(result)
      }
      
    },
    fail(error) {
      console.log('request fail', error);
      options.fail()
    }
  })
}





