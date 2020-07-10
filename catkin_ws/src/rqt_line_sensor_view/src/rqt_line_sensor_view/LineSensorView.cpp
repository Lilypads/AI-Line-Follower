#include "rqt_line_sensor_view/LineSensorView.h"

#include <pluginlib/class_list_macros.h>
#include <QString>

namespace linesensorview {

/* ========================================================================== */
/* Constructor/Destructor                                                     */
/* ========================================================================== */

LineSensorView::LineSensorView()
    : rqt_gui_cpp::Plugin(),
      widget_(nullptr) {

  setObjectName("LineSensorView");
}

/* ========================================================================== */
/* Initialize/Shutdown                                                        */
/* ========================================================================== */

void LineSensorView::initPlugin(qt_gui_cpp::PluginContext& context) {
  
  /* Could have a setup UI function... FIXME */
  
  widget_ = new QWidget();
  ui_.setupUi(widget_);
  if (context.serialNumber() > 1) {
    widget_->setWindowTitle(widget_->windowTitle() +
        " (" + QString::number(context.serialNumber()) + ")");
  }
  context.addWidget(widget_);
  
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    
    led_[i] = new QLed();
    led_[i]->setState(Qt::gray);
    ui_.ledLayout->addWidget(led_[i]);  
    
    displayValue_[i] = new QLineEdit();
    displayValue_[i]->setAlignment(Qt::AlignHCenter);
    displayValue_[i]->clear();
    displayValue_[i]->setReadOnly(true);
    displayValue_[i]->setText("4096");
    
    ui_.valuesLayout->addWidget(displayValue_[i]);
    
  }
  
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &LineSensorView::updateGuiValues);

  setupROS_();
  
  timer->start(100); //100ms
  
}

void LineSensorView::shutdownPlugin() {

  //.shutdown() for publishers and subscribers
  for(int i=0; i<NUMBER_OF_SENSORS; i++){
    valuesSub_[i].shutdown();
  }

}

/* ========================================================================== */
/* Settings                                                                   */
/* ========================================================================== */

void LineSensorView::saveSettings(
    qt_gui_cpp::Settings& plugin_settings,
    qt_gui_cpp::Settings& instance_settings) const {
}

void LineSensorView::restoreSettings(
    const qt_gui_cpp::Settings& plugin_settings,
    const qt_gui_cpp::Settings& instance_settings) {
}

/* ========================================================================== */
/* Methods                                                                    */
/* ========================================================================== */

void LineSensorView::setupROS_(){

    valuesSub_[0] = nh_.subscribe("mybot/left_sensor4/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 0));
    valuesSub_[1] = nh_.subscribe("mybot/left_sensor3/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 1));
    valuesSub_[2] = nh_.subscribe("mybot/left_sensor2/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 2));
    valuesSub_[3] = nh_.subscribe("mybot/left_sensor1/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 3));
    valuesSub_[4] = nh_.subscribe("mybot/right_sensor1/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 4));
    valuesSub_[5] = nh_.subscribe("mybot/right_sensor2/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 5));
    valuesSub_[6] = nh_.subscribe("mybot/right_sensor3/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 6));
    valuesSub_[7] = nh_.subscribe("mybot/right_sensor4/image_raw", 10, (boost::function<void(const sensor_msgs::Image::ConstPtr&)>)boost::bind(&LineSensorView::rawsensorValuesReceived_,this, _1, 7));

}

/* ========================================================================== */
/* Events                                                                     */
/* ========================================================================== */



/* ========================================================================== */
/* Callbacks                                                                  */
/* ========================================================================== */
  
void LineSensorView::rawsensorValuesReceived_(const sensor_msgs::Image::ConstPtr& msg, int index){
    sensorValues[index] = msg->data[0];
}


/* ========================================================================== */
/* Slots                                                                      */
/* ========================================================================== */

  void LineSensorView::updateGuiValues(){
  
    for(int i=0; i<NUMBER_OF_SENSORS; i++){
      
      uint8_t value = sensorValues[i];
      displayValue_[i]->setText(QString::number(value));
      led_[i]->setState(QColor(value,value,value));
    
    }
  
  }


/* ========================================================================== */
/* Signals                                                                    */
/* ========================================================================== */



} // namespace

PLUGINLIB_EXPORT_CLASS(linesensorview::LineSensorView,
                       rqt_gui_cpp::Plugin)
