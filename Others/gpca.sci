
A1 = {1,1,2,6,8,5; 4,8,6,3,5,7; 0,2,3,2,2,3; 1,1,2,6,8,5; 4,8,6,3,5,7; 0,2,3,2,2,3}
A2 = {2,3,8,1,1,2; 2,2,8,4,8,6; 1,5,3,0,2,3; 2,3,8,1,1,2; 2,2,8,4,8,6; 1,5,3,0,2,3}
A3 = {6,8,5,2,3,8; 3,5,7,2,2,8; 2,2,3,1,5,3; 6,8,5,2,3,8; 3,5,7,2,2,8; 2,2,3,1,5,3}

n=6;
for i = 1:n,
	for j = 1:n,
		M(i,j) = (A1(i,j) + A2(i,j) + A3(i,j))/3;
	end;
end
M

Atil1 = A1 - M
Atil2 = A2 - M
Atil3 = A3 - M

disp('--------------- Iteração 1 ---------------');

Lo = eye(6,3)

// Atil1' * Lo * Lo' * Atil1;
// Atil2' * Lo * Lo' * Atil2;
// Atil3' * Lo * Lo' * Atil3;
// Atil4' * Lo * Lo' * Atil4;
// Atil5' * Lo * Lo' * Atil5;
// Atil6' * Lo * Lo' * Atil6;

MR = ( Atil1' * Lo * Lo' * Atil1 ) + ( Atil2' * Lo * Lo' * Atil2 ) + ( Atil3' * Lo * Lo' * Atil3 )

[evals,X] = spec(MR)

R(:,1) = evals(:,6,:)
R(:,2) = evals(:,5,:)
R(:,3) = evals(:,4,:)

ML = ( Atil1 * R * R' * Atil1' ) + ( Atil2 * R * R' * Atil2' ) + ( Atil3 * R * R' * Atil3' )

[evals,Y] = spec(ML)

L(:,1) = evals(:,1,:)
L(:,2) = evals(:,2,:)
L(:,3) = evals(:,3,:)

RMSE1 = ( norm(Atil1 - L * L' * Atil1 * R * R', 'fro') + norm(Atil2 - L * L' * Atil2 * R * R', 'fro') + norm(Atil3 - L * L' * Atil3 * R * R', 'fro') ) / 3  

disp('--------------- Iteração 2 ---------------');

Lo = L

// Atil1' * Lo * Lo' * Atil1;
// Atil2' * Lo * Lo' * Atil2;
// Atil3' * Lo * Lo' * Atil3;
// Atil4' * Lo * Lo' * Atil4;
// Atil5' * Lo * Lo' * Atil5;
// Atil6' * Lo * Lo' * Atil6;

MR = ( Atil1' * Lo * Lo' * Atil1 ) + ( Atil2' * Lo * Lo' * Atil2 ) + ( Atil3' * Lo * Lo' * Atil3 )

[evals,X] = spec(MR)

R(:,1) = evals(:,1,:)
R(:,2) = evals(:,2,:)
R(:,3) = evals(:,3,:)

ML = ( Atil1 * R * R' * Atil1' ) + ( Atil2 * R * R' * Atil2' ) + ( Atil3 * R * R' * Atil3' )

[evals,Y] = spec(ML)

L(:,1) = evals(:,1,:)
L(:,2) = evals(:,2,:)
L(:,3) = evals(:,3,:)

RMSE2 = ( norm(Atil1 - L * L' * Atil1 * R * R', 'fro') + norm(Atil2 - L * L' * Atil2 * R * R', 'fro') + norm(Atil3 - L * L' * Atil3 * R * R', 'fro')) / 3 
RMSE2 - RMSE1

disp('--------------- Projeção ---------------')

D1 = L' * Atil1 * R
D2 = L' * Atil2 * R
D3 = L' * Atil3 * R
D4 = L' * Atil4 * R
D5 = L' * Atil5 * R
D6 = L' * Atil6 * R
