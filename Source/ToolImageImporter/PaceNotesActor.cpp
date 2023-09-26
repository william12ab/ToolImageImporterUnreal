
#include "PaceNotesActor.h"

#include "UObject/ConstructorHelpers.h"
APaceNotesActor::APaceNotesActor(){
	PrimaryActorTick.bCanEverTick = true;
	note_count = 0;
	play_value = 0.25f;
	is_played = false;
	is_playing = false;
	turn_counter = 0;
	turn_counter_called = 0;
	index_ = 0; 
	is_end_played = false;
		static ConstructorHelpers::FObjectFinder<USoundCue> strobj(TEXT("SoundCue'/Game/Sound/pacenotes/straight_Cue.straight_Cue'"));
		if (strobj.Succeeded()) {
			straight_cue = strobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> finobj(TEXT("SoundCue'/Game/Sound/pacenotes/and_finish_Cue.and_finish_Cue'"));
		if (finobj.Succeeded()) {
			finish_cue = finobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> hobj(TEXT("SoundCue'/Game/Sound/pacenotes/100_Cue.100_Cue'"));
		if (hobj.Succeeded()) {
			hunder_cue = hobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> hfobj(TEXT("SoundCue'/Game/Sound/pacenotes/150_Cue.150_Cue'"));
		if (hfobj.Succeeded()) {
			hunderf_cue = hfobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> thobj(TEXT("SoundCue'/Game/Sound/pacenotes/200_Cue.200_Cue'"));
		if (thobj.Succeeded()) {
			twohunder_cue = thobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> olobj(TEXT("SoundCue'/Game/Sound/pacenotes/1_left_Cue.1_left_Cue'"));
		if (olobj.Succeeded()) {
			one_l_cue = olobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> orobj(TEXT("SoundCue'/Game/Sound/pacenotes/1_right_Cue.1_right_Cue'"));
		if (orobj.Succeeded()) {
			one_r_cue = orobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> tobj(TEXT("SoundCue'/Game/Sound/pacenotes/2_left_Cue.2_left_Cue'"));
		if (tobj.Succeeded()) {
			two_l_cue = tobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> trobj(TEXT("SoundCue'/Game/Sound/pacenotes/2_right_Cue.2_right_Cue'"));
		if (trobj.Succeeded()) {
			two_r_cue = trobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> thlobj(TEXT("SoundCue'/Game/Sound/pacenotes/3_left_Cue.3_left_Cue'"));
		if (thlobj.Succeeded()) {
			three_l_cue = thlobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> throbj(TEXT("SoundCue'/Game/Sound/pacenotes/3_right_Cue.3_right_Cue'"));
		if (throbj.Succeeded()) {
			three_r_cue = throbj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> fobj(TEXT("SoundCue'/Game/Sound/pacenotes/4_left_Cue.4_left_Cue'"));
		if (fobj.Succeeded()) {
			four_l_cue = fobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> frobj(TEXT("SoundCue'/Game/Sound/pacenotes/4_right_Cue.4_right_Cue'"));
		if (frobj.Succeeded()) {
			four_r_cue = frobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> fvobj(TEXT("SoundCue'/Game/Sound/pacenotes/5_left_Cue.5_left_Cue'"));
		if (fvobj.Succeeded()) {
			five_l_cue = fvobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> fvrobj(TEXT("SoundCue'/Game/Sound/pacenotes/5_right_Cue.5_right_Cue'"));
		if (fvrobj.Succeeded()) {
			five_r_cue = fvrobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> sobj(TEXT("SoundCue'/Game/Sound/pacenotes/6_left_Cue.6_left_Cue'"));
		if (sobj.Succeeded()) {
			six_l_cue = sobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> srobj(TEXT("SoundCue'/Game/Sound/pacenotes/6_right_Cue.6_right_Cue'"));
		if (srobj.Succeeded()) {
			six_r_cue = srobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> hplobj(TEXT("SoundCue'/Game/Sound/pacenotes/hairpin_left_Cue.hairpin_left_Cue'"));
		if (hplobj.Succeeded()) {
			hp_l_cue = hplobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> hprobj(TEXT("SoundCue'/Game/Sound/pacenotes/hairpin_right_Cue.hairpin_right_Cue'"));
		if (hprobj.Succeeded()) {
			hp_r_cue = hprobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> sqobj(TEXT("SoundCue'/Game/Sound/pacenotes/hard_left_Cue.hard_left_Cue'"));
		if (sqobj.Succeeded()) {
			sq_l_cue = sqobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> sqrobj(TEXT("SoundCue'/Game/Sound/pacenotes/hard_right_Cue.hard_right_Cue'"));
		if (sqrobj.Succeeded()) {
			sq_r_cue = sqrobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> intobj(TEXT("SoundCue'/Game/Sound/pacenotes/into_Cue.into_Cue'"));
		if (intobj.Succeeded()) {
			into_cue = intobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> andobj(TEXT("SoundCue'/Game/Sound/pacenotes/and_Cue.and_Cue'"));
		if (andobj.Succeeded()) {
			and_cue = andobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> tightenobj(TEXT("SoundCue'/Game/Sound/pacenotes/tightens_Cue.tightens_Cue'"));
		if (tightenobj.Succeeded()) {
			tighten_cue = tightenobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> shortobj(TEXT("SoundCue'/Game/Sound/pacenotes/short_Cue.short_Cue'"));
		if (shortobj.Succeeded()) {
			short_cue = shortobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> restobj(TEXT("SoundCue'/Game/Sound/pacenotes/over_crest_Cue.over_crest_Cue'"));
		if (restobj.Succeeded()) {
			crest_cue = restobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> openobj(TEXT("SoundCue'/Game/Sound/pacenotes/opens_Cue.opens_Cue'"));
		if (openobj.Succeeded()) {
			open_cue = openobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> narrowobj(TEXT("SoundCue'/Game/Sound/pacenotes/narrow_archway_Cue.narrow_archway_Cue'"));
		if (narrowobj.Succeeded()) {
			narrow_cue = narrowobj.Object;
		}
		static ConstructorHelpers::FObjectFinder<USoundCue> longobj(TEXT("SoundCue'/Game/Sound/pacenotes/long_Cue.long_Cue'"));
		if (longobj.Succeeded()) {
			long_cue = longobj.Object;
		}

		static ConstructorHelpers::FObjectFinder<USoundCue> dipobj(TEXT("SoundCue'/Game/Sound/pacenotes/dip_Cue.dip_Cue'"));
		if (dipobj.Succeeded()) {
			dip_cue = dipobj.Object;
		}
	current_comp = CreateDefaultSubobject<UAudioComponent>(TEXT("currents"));
	current_comp->SetupAttachment(RootComponent);
	timert = 0.0f;
}

void APaceNotesActor::BeginPlay(){
	Super::BeginPlay();
}

void APaceNotesActor::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	Playing(DeltaTime);
}
void APaceNotesActor::FindDirection(const int& i, const int&n, const int& a) {
	if (directions_[i]>0){//right
		pacenotes_array[n] = a + 50;
	}
	else if (directions_[i] < 0){//left
		pacenotes_array[n] = a + 40;
	}
	else if (directions_[i] == 0){//same line
		//same line
	}
	//checks direction, modis most recent note, and changes for left and right
}
void APaceNotesActor::PlayNote(const float& DeltaTime, const int& index) {
	
	timert += DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("NOTE: %f"),timert);
	if (timert>2.0f){
		accum_time += DeltaTime;
		if (!is_playing) {
			current_comp->Activate();
			current_comp->SetSound(cues_[0]);
			current_comp->Play(0.f);
			is_playing = true;
		}
		if (accum_time > cues_[0]->GetDuration()) {
			cues_.RemoveAt(0);
			accum_time = 0.0f;
			is_playing = false;
		}
	}
}

void APaceNotesActor::Playing(float DeltaTime){
	if (cues_.Num()!=0){
		PlayNote(DeltaTime,0);
	}
}

void APaceNotesActor::FindAngle(int& i) {
	if (angles_[i] >= 180){//flat
		pacenotes_array.Add(17);
	}
	else if (angles_[i] >= 165 && angles_[i] < 180) {//6
		pacenotes_array.Add(16);
		FindDirection(i, pacenotes_array.Num()-1, 16);
	}
	else if (angles_[i] >= 150 && angles_[i] < 165) {//5
		pacenotes_array.Add(15);
		FindDirection(i, pacenotes_array.Num() - 1, 15);
	}
	else if (angles_[i] >= 135 && angles_[i] < 150) {//4
		pacenotes_array.Add(14);
		FindDirection(i, pacenotes_array.Num() - 1, 14);
	}
	else if (angles_[i] >= 120 && angles_[i] < 135) {//3
		pacenotes_array.Add(13);
		FindDirection(i, pacenotes_array.Num() - 1, 13);
	}
	else if (angles_[i] >= 105 && angles_[i] < 120) {//2
		pacenotes_array.Add(12);
		FindDirection(i, pacenotes_array.Num() - 1, 12);
	}
	else if (angles_[i] >= 91 && angles_[i] < 105) {//1
		pacenotes_array.Add(11);
		FindDirection(i, pacenotes_array.Num() - 1, 11);
	}
	else if (angles_[i] == 90 ) {//sq
		pacenotes_array.Add(10);
		FindDirection(i, pacenotes_array.Num() - 1, 10);
	}
	else if(angles_[i] <90 ) {//hp
		pacenotes_array.Add(9);
		FindDirection(i, pacenotes_array.Num() - 1, 19);
	}
	FindSmallDist(i);
}
void APaceNotesActor::FindSmallDist(int&i) {
	//checking if next length is short
	bool is_added = false;
	if (lengths_[i + 1] < 5) {
		pacenotes_array.Add(20);//tighten/widen
		is_added = true;
	}
	else if (lengths_[i + 1] >= 5 && lengths_[i + 1] < 10) {
		pacenotes_array.Add(21);//into
		is_added = true;
	}
	else if (lengths_[i + 1] >= 10&& lengths_[i + 1] < 15) {
		pacenotes_array.Add(22);//and
		is_added = true;
	}
	//find next angle too
	if (is_added) {
		if ((i + 1) < angles_.Num()) {
			i++;
			index_ = i;
			FindAngle(i);
		}
	}
}
void APaceNotesActor::FindOrder() {
	for (index_; index_ < lengths_.Num(); index_++){
		//lengths 150,100,200
		if (lengths_[index_] > (45) && lengths_[index_] < (70)) {
			pacenotes_array.Add(1);
		}
		else if (lengths_[index_] >= (70) && lengths_[index_] < (95)) {
			pacenotes_array.Add(2);
		}
		else if (lengths_[index_] >= (95) && lengths_[index_] < (105)) {
			pacenotes_array.Add(3);
		}
		//angles
		if (index_ <(angles_.Num())){
			FindAngle(index_);
		}
	}
	pacenotes_array.Add(23);
	for (int i = 0; i < pacenotes_array.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("NOTE: %d"), pacenotes_array[i]);
	}
}

void APaceNotesActor::WhenToPlay(const FVector2D&p1, const FVector2D& p2, const FVector2D& p3) {
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
	if (t> play_value){
		if (!is_played){
			if (turn_counter == turn_counter_called||turn_counter==0|| turn_counter_called==0) {
				if (note_count < pacenotes_array.Num()) {
					PlayNextNote();
					note_count++;
					is_played = true;
				}
			}
		}
	}
}

void APaceNotesActor::PlayFirstNote() {
	PlayNextNote();
	note_count++;
	is_played = true;
}

void APaceNotesActor::SetForEnd() {
	if (pacenotes_array[pacenotes_array.Num()-1]==22|| pacenotes_array[pacenotes_array.Num() - 1]==20|| pacenotes_array[pacenotes_array.Num() - 1] == 20 || 21){
		pacenotes_array[pacenotes_array.Num() - 1] = 23;
	}
	else {
		pacenotes_array.Add(23);
	}
}

void APaceNotesActor::AddSwitch(const int& note,USoundCue*cue_,const bool& is_addition_) {
	cues_.Add(cue_);
	notes_to_display.Add(note);
}
void APaceNotesActor::EmptyArray() {
	notes_to_display.Empty();
}

void APaceNotesActor::PlayNextNote() {
	if (note_count < pacenotes_array.Num()) {
		//UE_LOG(LogTemp, Warning, TEXT("NOTE: %d"), note_count);
		switch (pacenotes_array[note_count])
		{
		case 17: {//straight
			turn_counter_called++;
			AddSwitch(22,six_l_cue,true);
			break;
		}
		case 1: {//100
			note_count++;
			AddSwitch(1, hunder_cue, false);
			PlayNextNote();
			break;
		}
		case 2: {//150
			note_count++;
			AddSwitch(2, hunderf_cue, false);
			PlayNextNote();
			break;
		}
		case 3: {//200
			note_count++;
			AddSwitch(3, twohunder_cue, false);
			PlayNextNote();
			break;
		}
		case 56: {//l6
			turn_counter_called++;
			AddSwitch(22, six_l_cue, false);
			break;
		}
		case 55: {//l5
			turn_counter_called++;
			AddSwitch(20, five_l_cue, false);
			break;
		}
		case 54: {//l4
			turn_counter_called++;
			AddSwitch(18, four_l_cue, false);
			break;
		}
		case 53: {//l3
			turn_counter_called++;
			AddSwitch(16, three_l_cue, false);
			break;
		}
		case 52: {//l2
			turn_counter_called++;
			AddSwitch(14, two_l_cue, false);
			break;
		}
		case 51: {//l1
			turn_counter_called++;
			AddSwitch(12, one_l_cue, false);
			break;
		}
		case 50: {//sql
			turn_counter_called++;
			AddSwitch(10, sq_l_cue, false);
			break;
		}
		case 59: {//hpl
			turn_counter_called++;
			AddSwitch(5, hp_l_cue, false);
			break;
		}
		case 66: {//r6
			turn_counter_called++;
			AddSwitch(23, six_r_cue, false);
			break;
		}
		case 65: {//r5		
			turn_counter_called++;
			AddSwitch(21, five_r_cue, false);
			break;
		}
		case 64: {//r4
			turn_counter_called++;
			AddSwitch(19, four_r_cue, false);
			break;
		}
		case 63: {//r3
			turn_counter_called++;
			AddSwitch(17, three_r_cue, false);
			break;
		}
		case 62: {//r2
			turn_counter_called++;
			AddSwitch(15, two_r_cue, false);
			break;
		}
		case 61: {//r1
			turn_counter_called++;
			AddSwitch(13, one_r_cue, false);
			break;
		}
		case 60: {//sqr
			turn_counter_called++;
			AddSwitch(9, sq_r_cue, false);
			break;
		}
		case 69: {//hpr
			turn_counter_called++;
			AddSwitch(6, hp_r_cue, false);
			break;
		}
		}
	}
	PlayAddition();
}

void APaceNotesActor::RemoveSix() {
	notes_to_display.RemoveAt(0, 6);
}

void APaceNotesActor::PlayAddition() {
	bool is_addition = false;
	if (note_count + 1 < pacenotes_array.Num()) {
		switch (pacenotes_array[note_count + 1])
		{
		case 20: {
			is_addition = true;
			//tightens/widens
			note_count++;
			AddSwitch(11, tighten_cue, true);
			break;
		}
		case 21: {
			is_addition = true;
			//into
			note_count++;
			AddSwitch(7, into_cue, true);
			break;
		}
		case 22: {
			is_addition = true;
			//and
			note_count++;
			AddSwitch(4, and_cue, true);
			break;
		}
		case 23: {
			AddSwitch(24, finish_cue, true);
			note_count++;
			is_addition = false;
			break;
		}
		}
	}
	if (is_addition){
		note_count++;
		PlayNextNote();
	}
}