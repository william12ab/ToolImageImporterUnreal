
#include "PaceNotesActor.h"
APaceNotesActor::APaceNotesActor(){
	PrimaryActorTick.bCanEverTick = true;
}

void APaceNotesActor::BeginPlay(){
	Super::BeginPlay();
}

void APaceNotesActor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void APaceNotesActor::FindAngle(const int& i) {
	if (angles_[i] >= 0 && angles_[i] < 20) {
		pacenotes_array.Add(16);
	}
	else if (angles_[i] >= 20 && angles_[i] < 35) {
		pacenotes_array.Add(15);
	}
	else if (angles_[i] >= 35 && angles_[i] < 50) {
		pacenotes_array.Add(14);
	}
	else if (angles_[i] >= 50 && angles_[i] < 65) {
		pacenotes_array.Add(13);
	}
	else if (angles_[i] >= 65 && angles_[i] < 75) {
		pacenotes_array.Add(12);
	}
	else if (angles_[i] >= 75 && angles_[i] < 89) {
		pacenotes_array.Add(11);
	}
	else if (angles_[i] >= 89) {
		pacenotes_array.Add(10);
	}
}

void APaceNotesActor::FindOrder() {
	for (size_t i = 0; i < lengths_.Num(); i++){
		//lengths 150,100,200
		if (lengths_[i] > (90/2) && lengths_[i] < (110/2)) {
			pacenotes_array.Add(1);
		}
		else if (lengths_[i] > (140/2) && lengths_[i] < (160/2)) {
			pacenotes_array.Add(2);
		}
		else if (lengths_[i] > (190/2) && lengths_[i] < (210/2)) {
			pacenotes_array.Add(3);
		}

		//angles
		if (i<(angles_.Num())){
			FindAngle(i);
			//checking if next length is short
			bool is_added = false;
			if (lengths_[i+1]<5){
				pacenotes_array.Add(20);//tighten/widen
				is_added = true;
			}
			else if (lengths_[i+1]>=5 && lengths_[i + 1]<15){
				pacenotes_array.Add(21);//into
				is_added = true;
			}
			else if (lengths_[i + 1] >= 15 && lengths_[i + 1] < 25) {
				pacenotes_array.Add(22);//and
				is_added = true;
			}
			//find next angle too
			if (is_added ){
				if ((i+1)<angles_.Num()){
					FindAngle(i + 1);
					i++;
				}
			}
		}
	}
}

void APaceNotesActor::WhenToPlay() {
	int test = 2;
}