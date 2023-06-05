#include"LayerManage.h"


void LayerManage::addLayer(Layer* layer) {
	layers.push_back(layer);
}

void LayerManage::deleteLayer(Layer* layer) {
	QVector<Layer*>::iterator it = qFind(layers.begin(), layers.end(), layer);
	if (it != layers.end()) {
		layers.erase(it);
		delete layer;
	}
}
void LayerManage::setActiveLayer(Layer* layer) {
	QVector<Layer*>::iterator it = qFind(layers.begin(), layers.end(), layer);
	if (it != layers.end()) {
        active = layer;
		for (it = layers.begin(); it != layers.end(); it++) {
			if (*it != layer) {
				(*it)->setActive(false);
			}
		}
	   layer->setActive(true);
	}
	
}
Layer* LayerManage::getActiveLayer() {
	return active;
}
QVector<Layer*> &LayerManage::getLayers() {
	return layers;
}