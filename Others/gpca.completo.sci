
A1 = {1,1,2;4,8,6;0,2,3}
A2 = {6,8,5;3,5,7;2,2,3}
A3 = {2,3,8;2,2,8;1,5,3}
A4 = {1,1,2;4,8,6;0,2,3}
A5 = {6,8,5;3,5,7;2,2,3}
A6 = {2,3,8;2,2,8;1,5,3}

n=3;
for i = 1:n,
	for j = 1:n,
		M(i,j) = (A1(i,j) + A2(i,j) + A3(i,j) + A4(i,j) + A5(i,j) + A6(i,j))/6;
	end;
end
M

Atil1 = A1 - M
Atil2 = A2 - M
Atil3 = A3 - M
Atil4 = A4 - M
Atil5 = A5 - M
Atil6 = A6 - M

disp('--------------- Iteração 1 ---------------')

Lo = {1,0;0,1;0,0}

Atil1' * Lo * Lo' * Atil1
Atil2' * Lo * Lo' * Atil2
Atil3' * Lo * Lo' * Atil3
Atil4' * Lo * Lo' * Atil4
Atil5' * Lo * Lo' * Atil5
Atil6' * Lo * Lo' * Atil6

MR = ( Atil1' * Lo * Lo' * Atil1 ) + ( Atil2' * Lo * Lo' * Atil2 ) + ( Atil3' * Lo * Lo' * Atil3 ) + ( Atil4' * Lo * Lo' * Atil4 ) + ( Atil5' * Lo * Lo' * Atil5 ) + ( Atil6' * Lo * Lo' * Atil6 )

[evals,X] = spec(MR)

R(:,1) = evals(:,3,:)
R(:,2) = evals(:,2,:)

ML = ( Atil1 * R * R' * Atil1' ) + ( Atil2 * R * R' * Atil2' ) + ( Atil3 * R * R' * Atil3' ) + ( Atil4 * R * R' * Atil4' ) + ( Atil5 * R * R' * Atil5' ) + ( Atil6 * R * R' * Atil6' )

[evals,Y] = spec(ML)

L(:,1) = evals(:,1,:)
L(:,2) = evals(:,2,:)

RMSE1 = ( norm(Atil1 - L * L' * Atil1 * R * R', 'fro') + norm(Atil2 - L * L' * Atil2 * R * R', 'fro') + norm(Atil3 - L * L' * Atil3 * R * R', 'fro') + norm(Atil4 - L * L' * Atil4 * R * R', 'fro') + norm(Atil5 - L * L' * Atil5 * R * R', 'fro') + norm(Atil6 - L * L' * Atil6 * R * R', 'fro') ) / 6 

disp('--------------- Iteração 2 ---------------')

Lo = L

Atil1' * Lo * Lo' * Atil1
Atil2' * Lo * Lo' * Atil2
Atil3' * Lo * Lo' * Atil3
Atil4' * Lo * Lo' * Atil4
Atil5' * Lo * Lo' * Atil5
Atil6' * Lo * Lo' * Atil6

MR = ( Atil1' * Lo * Lo' * Atil1 ) + ( Atil2' * Lo * Lo' * Atil2 ) + ( Atil3' * Lo * Lo' * Atil3 ) + ( Atil4' * Lo * Lo' * Atil4 ) + ( Atil5' * Lo * Lo' * Atil5 ) + ( Atil6' * Lo * Lo' * Atil6 )

[evals,X] = spec(MR)

R(:,1) = evals(:,1,:)
R(:,2) = evals(:,3,:)

ML = ( Atil1 * R * R' * Atil1' ) + ( Atil2 * R * R' * Atil2' ) + ( Atil3 * R * R' * Atil3' ) + ( Atil4 * R * R' * Atil4' ) + ( Atil5 * R * R' * Atil5' ) + ( Atil6 * R * R' * Atil6' )

[evals,Y] = spec(ML)

L(:,1) = evals(:,1,:)
L(:,2) = evals(:,2,:)

RMSE2 = ( norm(Atil1 - L * L' * Atil1 * R * R', 'fro') + norm(Atil2 - L * L' * Atil2 * R * R', 'fro') + norm(Atil3 - L * L' * Atil3 * R * R', 'fro') + norm(Atil4 - L * L' * Atil4 * R * R', 'fro') + norm(Atil5 - L * L' * Atil5 * R * R', 'fro') + norm(Atil6 - L * L' * Atil6 * R * R', 'fro') ) / 6 

disp('--------------- Projeção ---------------')

D1 = L' * Atil1 * R
D2 = L' * Atil2 * R
D3 = L' * Atil3 * R
D4 = L' * Atil4 * R
D5 = L' * Atil5 * R
D6 = L' * Atil6 * R

