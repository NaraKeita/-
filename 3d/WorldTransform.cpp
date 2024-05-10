#include "WorldTransform.h"

<<<<<<< HEAD
/* void WorldTransform::UpdateMatrix() {
	//	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}*/
=======
void WorldTransform::UpdateMatrix() { 
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);
	TransferMatrix();
}
>>>>>>> 1636c01c0139319c35fc1f9b4ea00a54051bfd02
