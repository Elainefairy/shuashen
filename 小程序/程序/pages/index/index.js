//index.js
//获取应用实例
var app;
var util = require('../../utils/util.js')
var ocr = require('../../utils/ocr.js')
var imgHelper = require('../../utils/img_helper.js')
Page({
  /*data: {
    motto: 'Hello World',
    userInfo: {},
    hasUserInfo: false,
    canIUse: wx.canIUse('button.open-type.getUserInfo')
  },
  //事件处理函数
  bindViewTap: function() {
    wx.navigateTo({
      url: '../logs/logs'
    })
  },
  onLoad: function () {
    if (app.globalData.userInfo) {
      this.setData({
        userInfo: app.globalData.userInfo,
        hasUserInfo: true
      })
    } else if (this.data.canIUse){
      // 由于 getUserInfo 是网络请求，可能会在 Page.onLoad 之后才返回
      // 所以此处加入 callback 以防止这种情况
      app.userInfoReadyCallback = res => {
        this.setData({
          userInfo: res.userInfo,
          hasUserInfo: true
        })
      }
    } else {
      // 在没有 open-type=getUserInfo 版本的兼容处理
      wx.getUserInfo({
        success: res => {
          app.globalData.userInfo = res.userInfo
          this.setData({
            userInfo: res.userInfo,
            hasUserInfo: true
          })
        }
      })
    }
  },
  getUserInfo: function(e) {
    console.log(e)
    app.globalData.userInfo = e.detail.userInfo
    this.setData({
      userInfo: e.detail.userInfo,
      hasUserInfo: true
    })
  }*/
  data: {
   
    DESC: [
      {
        title: true,
        desc: '要求'
      },
      {
        desc: '图片小于3M'
      },
      {
        desc: '分辨率低于4k'
      },
      {
        desc: '保证清晰度'
      }

    ],
    animScope: {
      DESC: true,
      obtainImgInit: true,
      bankInfoInit: true,
      copyInit: true
    },
    imageUrl: "https://s1.ax1x.com/2018/06/22/Pp3njs.jpg" 
     


  },
  // 重置显示结果动画
  resetResultAnim: function () {
    this.setData({
      animScope: {
        bankInfoInit: true,
        copyInit: true
      }
    })



  },



  onLoad: function (res) {
    app = getApp()
  },
 
  animDESC: function () {
    // 描述动画
    var animDESC = wx.createAnimation({
      timingFunction: 'ease',
    })
    animDESC.opacity(1).step({ duration: 2000 })
    this.setData({
      animDESC: animDESC.export()
    })
  },

  animObtainImgStart: function () {
    // 初始选择照片按钮动画
    var animObtainImgStart = wx.createAnimation({
      timingFunction: 'ease',
    })
    animObtainImgStart.scale(0.8, 0.8).step({ duration: 300, delay: 200 })
    animObtainImgStart.scale(1.2, 1.2).step({ duration: 200 })
    animObtainImgStart.scale(1, 1).step({ duration: 300 })
    this.setData({
      animObtainImgStart: animObtainImgStart.export()
    })
  },

  animObtainImg: function (reserve) {
    // 获取照片动画
    var animObtainImg = wx.createAnimation({
      timingFunction: 'ease',
    })
    animObtainImg.opacity(reserve ? 0 : 1).step({ duration: 2000 })
    this.setData({
      animObtainImg: animObtainImg.export()
    })
  },

  onShow: function () {

    // 初始描述文字动画
    this.animDESC()

    // 初始按钮动画
    this.animObtainImgStart()

    this.resetResultAnim()

  },

  // 预览图片
  previewImg: function () {
    /* wx.previewImage({
       current: this.data.imgFile.path,
       urls: [this.data.imgFile.path]
     })*/
    var current = e.target.dataset.src;
    wx.previewImage({
      current: current, // 当前显示图片的http链接  
      urls: this.data.imgalist // 需要预览的图片http链接列表  
    })
  },

  chooseImage: function () {
    var that = this
    wx.chooseImage({
      count: 1, // 默认9
      sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
      sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
      success: function (res) {

        // 重置动画
        that.resetResultAnim()
        // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
        var imgFile = res.tempFiles[0]
        that.setData({
          imgFile: imgFile,
          // bankInfo: false
        })

        // 显示获取人参照片模块
        that.animObtainImg()

        console.log(imgFile.size / 1024 + 'kb')
        if (res.size > 1024 * 1024 * 3) {
          util.showModel('错误', '图片需要小于 3M')
        } else {
          //
          //that.ocrOption(imgFile.path);
          uploade_file(
            "https://ecnu-iot.emlab.net/upload", 
            "image_file",
            imgFile.path,
            {"product_id":"1"},
            function success(res) {
              console.log("upload success, res = " + res)


              if (res.indexOf('error')!=-1)
              {
                util.showModel('提示', '匹配失败，是赝品！')
              }else{
                util.showModel('提示', '匹配成功，是真品！')
              }
              },
            function fail(res) { 
              console.log("upload fail")
             // previewImg
             
            },
           )
        }
      }
    })
  },

  // ocr识别
  ocrOption: function (img) {
    util.showBusy("OCR识别中...")
    var that = this
    var ret = ocr.getBankNumber({ 
      imgPath: img,
      success: function (result) {
        wx.hideLoading();
        if (result) {
          that.setData({
            bankInfo: result
          })
          that.anim4result()
        } else {
          util.showModel('提示', '未识别到人参信息')//  这是什么鬼！！！
        }

      },
      fail: function () {
        wx.hideLoading();
        util.showModel('提示', '识别失败')
      }
    })
  },
  // 预览图片
  previewImg: function () {
   /* wx.previewImage({
      current: this.data.imgFile.path,
      urls: [this.data.imgFile.path]
    })*/
    var current = e.target.dataset.src;
    wx.previewImage({
      current: current, // 当前显示图片的http链接  
      urls: this.data.imgalist // 需要预览的图片http链接列表  
    })  
  }
})

function uploade_file(url, name, filePath, formData, success, fail) {
  console.log('a=' + filePath)
  wx.uploadFile({
    url: url,
    filePath: filePath,
    name: name,
    method: 'POST',
    formData: formData, // HTTP 请求中其他额外的 form data
    success: function (res) {
      console.log(res);
      if (res.statusCode == 200 && !res.data.result_code) {
        typeof success == "function" && success(res.data);
      } else {
        typeof fail == "function" && fail(res);
      }
    },
    fail: function (res) {
      console.log(res);
      typeof fail == "function" && fail(res);
    }
  })
}