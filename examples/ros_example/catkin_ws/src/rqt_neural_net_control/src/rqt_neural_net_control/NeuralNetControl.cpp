#include "rqt_neural_net_control/NeuralNetControl.h"

#include <pluginlib/class_list_macros.h>

namespace neuralnetcontrol {

/* ========================================================================== */
/* Constructor/Destructor                                                     */
/* ========================================================================== */

NeuralNetControl::NeuralNetControl()
    : rqt_gui_cpp::Plugin(),
      widget_(nullptr) {

  setObjectName("NeuralNetControl");
}

/* ========================================================================== */
/* Initialize/Shutdown                                                        */
/* ========================================================================== */

void NeuralNetControl::initPlugin(qt_gui_cpp::PluginContext& context) {
  widget_ = new QWidget();
  ui_.setupUi(widget_);
  if (context.serialNumber() > 1) {
    widget_->setWindowTitle(widget_->windowTitle() +
        " (" + QString::number(context.serialNumber()) + ")");
  }
  context.addWidget(widget_);
  
  //Loss function options
  ui_.lossFunctionComboBox->addItems(lossFunctions);
  
  //Optimiser options
  ui_.optimiserComboBox->addItems(optimiserFunctions);
  
  //convolutional net
  ui_.convolutionalComboBox->addItems(convolutionOptions);
  
  connect(ui_.addLayerButton, SIGNAL(released()), this, SLOT(addLayer()));
  connect(ui_.deleteLayerButton, SIGNAL(released()), this, SLOT(deleteLayer()));
  
  //setup the service handling
  char topic_name[] = "neural_net_config";
  get_config_service = nh_.advertiseService(topic_name, &NeuralNetControl::GetNeuralNetworkConfig_callback, this);
  
}

void NeuralNetControl::shutdownPlugin() {

  //TODO deallocate any memory that is left?
  
  get_config_service.shutdown();

}

/* ========================================================================== */
/* Settings                                                                   */
/* ========================================================================== */

void NeuralNetControl::saveSettings(
    qt_gui_cpp::Settings& plugin_settings,
    qt_gui_cpp::Settings& instance_settings) const {
}

void NeuralNetControl::restoreSettings(
    const qt_gui_cpp::Settings& plugin_settings,
    const qt_gui_cpp::Settings& instance_settings) {
}

/* ========================================================================== */
/* Methods                                                                    */
/* ========================================================================== */



/* ========================================================================== */
/* Events                                                                     */
/* ========================================================================== */



/* ========================================================================== */
/* Callbacks                                                                  */
/* ========================================================================== */
  
  bool NeuralNetControl::GetNeuralNetworkConfig_callback(rqt_neural_net_control::GetNeuralNetConfig::Request &req,
                                       rqt_neural_net_control::GetNeuralNetConfig::Response &res)
  {                                     

    //TODO maybe put the bigger chunks in their own functions?

    //learning rate
    res.learning_rate = ui_.learningRateBox->value();
    
    //loss function
    switch(ui_.lossFunctionComboBox->currentIndex()){
      case 0:
        res.loss_function = res.CROSS_ENTROPY; break;
      case 1:
        res.loss_function = res.MEAN_SQUARED_ERROR; break;
      default:
        break;
    }
    
    //optimiser
    switch(ui_.optimiserComboBox->currentIndex()){
      case 0:
        res.optimiser = res.STOCHASTIC_GRADIENT_DECENT; break;
      case 1:
        res.optimiser = res.MOMENTUM; break;
      case 2:
        res.optimiser = res.ADAGRAD; break;
      case 3:
        res.optimiser = res.RMSPROP; break;
      case 4:
        res.optimiser = res.ADAM; break;
      case 5:
        res.optimiser = res.ADAMAX; break;
      case 6:
        res.optimiser = res.GRADIENT_DECENT; break;  
      default:
        break;
    }
    
    //convolutional_net_enabled
    switch(ui_.convolutionalComboBox->currentIndex()){
      case 0:
        res.convolutional_net_enabled = true; break;
      case 1:
        res.convolutional_net_enabled = false; break;
      default:
        break;
    }
    
    //number_of_layers
    res.number_of_layers = layerEntries.size();
    
    //layer_activations
    for(int i=0; i<layerEntries.size(); i++){
      
      switch(layerEntriesComboBox[i]->currentIndex()){
        case 0:
          res.layer_activations.push_back(res.TANH); break;
        case 1:
          res.layer_activations.push_back(res.ASINH); break;
        case 2:
          res.layer_activations.push_back(res.SIGMOID); break;
        case 3:
          res.layer_activations.push_back(res.RELU); break;
        case 4:
          res.layer_activations.push_back(res.LEAKY_RELU); break;
        case 5:
          res.layer_activations.push_back(res.SOFTMAX); break;
        case 6:
          res.layer_activations.push_back(res.LINEAR); break;        
        default:
          break;
      } 
      
      res.layer_neuron_count.push_back(layerEntriesSpinBox[i]->value());
      
    }
    
    //multipliers
    res.input_multiplier = ui_.inputMultiplier->value();
    res.output_multiplier = ui_.outputMultiplier->value();
    res.closed_loop_error_multiplier = ui_.errorMultiplier->value();

    return true;                       
  }


/* ========================================================================== */
/* Slots                                                                      */
/* ========================================================================== */

  void NeuralNetControl::addLayer(){
    
    QHBoxLayout* newLayerLayout = new QHBoxLayout;
    QLabel* newLayerLabel = new QLabel;
    QComboBox* newLayerComboBox = new QComboBox;
    QSpinBox* newSpinBox = new QSpinBox;
    
    newLayerLabel->setText("Layer " + QString::number(layerEntries.size()) + ".");
    newLayerLabel->setAlignment(Qt::AlignHCenter);
    
    newLayerComboBox->addItems(activationFunctions);
      
    newLayerLayout->addWidget(newLayerLabel);
    newLayerLayout->addWidget(newSpinBox);
    newLayerLayout->addWidget(newLayerComboBox);
    
    layerEntries.push_back(newLayerLayout);
    layerEntriesComboBox.push_back(newLayerComboBox);
    layerEntriesSpinBox.push_back(newSpinBox);

    ui_.mainLayersLayout->addLayout(newLayerLayout);
    
  }

  void NeuralNetControl::deleteLayer(){
  
    if(layerEntries.size()){
    
      QHBoxLayout* deleteLayerLayout = layerEntries[layerEntries.size()-1];
      
      QLayoutItem *child;
      while ((child = deleteLayerLayout->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
      }
     
      //remove the layout from the main layout
      ui_.mainLayersLayout->removeItem(deleteLayerLayout);
      delete deleteLayerLayout;

      //remove from the vector...
      layerEntries.pop_back();
      layerEntriesComboBox.pop_back();
      layerEntriesSpinBox.pop_back();
    
    }
    
  }
  

/* ========================================================================== */
/* Signals                                                                    */
/* ========================================================================== */



} // namespace

PLUGINLIB_EXPORT_CLASS(neuralnetcontrol::NeuralNetControl,
                       rqt_gui_cpp::Plugin)
