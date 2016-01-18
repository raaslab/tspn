function [T,O,len] = compute_TSP_tour(X,path)

% Computes a Traveling Salesman Tour of a set of points.
%
% THIS ASSUMES THAT X IS A COMPLETE EUCLIDEAN 2D GRAPH.
%
% This function is a wrapper that makes a call to the Concorde TSP solver.
% Download Concorde from: http://www.tsp.gatech.edu/concorde/downloads/downloads.htm
% Install with either QSopt or other solver
%
% function [T,O] = compute_TSP_tour(X,path)
% 	X: nx2 set of input points
% 	path: path to the "./concorde" executable
% 
% 	T: points in X ordered according to the TSP tour
% 	O: order of indices in X such that X(O(i),:) is the i'th point in the tour.
%	len: length of the final tour

% Must contain at least three points
n = length(X);
if n < 3
	error('Must contain at least three points');
	return;
end

% Check if the rows and columns need to be swapped
if(size(X,1) < size(X,2))
	X = X';
end

% Concorde rounds the distances to integers
% So if points have distances less than 0.5 units, they will become 0
% To avoid this, I am going to scale all the distances
% Right now, I am just going to scale by 100
% TODO a better way of scaling, do this only if necessary
scaled_X = 100*X;


% Open the temporary tsplib format file
fid = fopen('temp.tsp','W');
if fid < 0
	error('Cannot create temp file');
	return;
end


% Write the header data to the file
fprintf(fid, ['NAME : temp\n' ...
							'TYPE : TSP\n' ...
							'COMMENT : temp\n' ...
							'DIMENSION : ' num2str(n) '\n' ...
							'EDGE_WEIGHT_TYPE : EUC_2D\n' ...
							'NODE_COORD_TYPE : TWOD_COORDS\n' ...
							'NODE_COORD_SECTION : \n']);

% Write out the coordinates
for i = 1 : n
	fprintf(fid, [num2str(i-1) ' ' num2str(scaled_X(i,1)) ' ' num2str(scaled_X(i,2)) '\n']); 
end

fclose(fid);

% Call the concorde solver
cmd = [path ' -x temp.tsp'];
system(cmd);

% Read in results
fid = fopen('temp.sol','r');
if fid < 0
	error('Cannot open output file.');
	return;
end

[O] = fscanf(fid,'%d');
O = O(2:end) + 1; 		% concorde starts indices at 0 instead of 1
T = X(O,:);

len = 0;
for i = 1 : n-1
	len = len + norm(T(i,:)-T(i+1,:));
end
len = len + norm(T(end,:)-T(1,:));
