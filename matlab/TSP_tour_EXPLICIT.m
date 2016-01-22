function [O] = TSP_tour_EXPLICIT(graph,path) % 'graph' - shoudld be a complete graph if incomplete  edges which u don't want should  have cost more than sum of the weights
                                                    % 'path' - '/home/ashishkb/softwares/concorde/concorde_build/TSP/concorde' like this 
                                                    
                                                    

% Computes a Traveling Salesman Tour of a set of points.
%
%
% 	T: points in X ordered according to the TSP tour
% 	O: order of indices in X such that X(O(i),:) is the i'th point in the tour.
%	len: length of the final tour

% Must contain at least three points
n = length(graph);
if n < 3
	error('Must contain at least three points');
	return;
end

% Check if the rows and columns need to be swapped

% Concorde rounds the distances to integers
% So if points have distances less than 0.5 units, they will become 0
% To avoid this, I am going to scale all the distances
% Right now, I am just going to scale by 100
% TODO a better way of scaling, do this only if necessary


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
							'EDGE_WEIGHT_TYPE : EXPLICIT\n' ...
                            'EDGE_WEIGHT_FORMAT :  FULL_MATRIX\n' ...
                            'NODE_COORD_TYPE :  NO_COORDS\n' ...
                            'EDGE_WEIGHT_SECTION : \n']);

% Write out the coordinates

fprintf(fid, [repmat(' %d ', 1, size(graph,1)) '\n'], graph');

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
% T = X(O,:);
% 
% len = 0;
% for i = 1 : n-1
% 	len = len + norm(T(i,:)-T(i+1,:));
% end
% len = len + norm(T(end,:)-T(1,:));
