
#include "PaceNotesActor.h"
APaceNotesActor::APaceNotesActor(){
	PrimaryActorTick.bCanEverTick = true;
	note_count = 0;
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
	else if (angles_[i] >= 89 && angles_[i]<91) {
		pacenotes_array.Add(10);
	}
	else if(angles_[i] >= 91 ) {
		pacenotes_array.Add(9);
	}
}

void APaceNotesActor::FindOrder() {
	for (size_t i = 0; i < lengths_.Num(); i++){
		//lengths 150,100,200
		if (lengths_[i] > (45) && lengths_[i] < (70)) {
			pacenotes_array.Add(1);
		}
		else if (lengths_[i] >= (70) && lengths_[i] < (95)) {
			pacenotes_array.Add(2);
		}
		else if (lengths_[i] >= (95) && lengths_[i] < (105)) {
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

void APaceNotesActor::WhenToPlay(const FVector&p1, const FVector& p2, const FVector& p3) {
	auto a = p1 - p3;
	auto b = p1 - p2;

	float t = 0.0f;
	if (a.X == 0 || b.X == 0){
		float c2 = ((float)a.Y / (float)b.Y);
		t = c2;
		if (t < 0.0f){
			t *= -1;
		}
	}
	else if (a.Y == 0 || b.Y == 0){
		float c1 = ((float)a.X/ (float)b.X);
		t = c1;
	}
	else{
		float c1 = ((float)a.X / (float)b.X);
		float c2 = ((float)a.Y / (float)b.Y);
		t = (c1 + c2) / 2.0f;
	}
	//above works out t

	if (t< play_value){
		PlayNextNote();
		PlayAddition();
		note_count++;
	}
}

void APaceNotesActor::PlayNextNote() {
	switch (pacenotes_array[note_count])
	{
	case 1: {
		break;
	}
	case 2: {
		break;
	}
	case 3: {
		break;
	}
	case 16: {
		break;
	}
	case 15: {
		break;
	}
	case 14: {
		break;
	}
	case 13: {
		break;
	}
	case 12: {
		break;
	}
	case 11: {
		break;
	}
	case 10: {
		break;
	}
	case 9: {
		break;
	}
	}
}

void APaceNotesActor::PlayAddition() {
	bool is_addition = false;
	switch (pacenotes_array[note_count])
	{
	case 20: {
		is_addition = true;
	}
	case 21: {
		is_addition = true;
	}
	case 22: {
		is_addition = true;
	}
	}
	if (is_addition){
		note_count++;
		PlayNextNote();
	}
}