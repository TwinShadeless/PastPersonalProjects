using System;
using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;
using UnityEngine.Tilemaps;
using UnityEngine.UI;

public class GridManager : MonoBehaviour
{
    [SerializeField] private int width, height, mines, tilesLeft;
    [SerializeField] public int minesLeft;
    [SerializeField] private Tile TilePrefab;
    [SerializeField] private Transform cam;
    [SerializeField] private Camera camObject;
    [SerializeField] private List<List<Tile>> gridMemory = new List<List<Tile>>();
    [SerializeField] private Sprite[] TileSprites = new Sprite[11];
    [SerializeField] public bool lost = false;
    [SerializeField] public TMP_Text MinesUI;

    private void Start()
    {
        GenerateGrid();
    }
    void GenerateGrid()
    {
        //Generating a grid of TilePrefabs, size width:height, and placing each Tile in a 2D List for further editing
        for (int i = 0; i < width; i++)
        {
            MinesUI.text = "Mines: " + mines.ToString();
            gridMemory.Add(new List<Tile>());
            for (int j = 0; j < height; j++)
            {
                var spawnedTile = Instantiate(TilePrefab, new Vector3(i,j),Quaternion.identity);
                spawnedTile.name = $"Tile({i},{j})";
                spawnedTile.grid = this;
                spawnedTile.x = i;
                spawnedTile.y = j;
                gridMemory[i].Add(spawnedTile);
            }
        }
        tilesLeft = width * height - mines;

        //Placing mines randomly across the grid
        minesLeft = mines;
        int x, y;
        while (minesLeft > 0)
        {
            x = UnityEngine.Random.Range(0, width);
            y = UnityEngine.Random.Range(0, height);
            if (gridMemory[x][y].val == 0)
            {
                minesLeft--;
                gridMemory[x][y].val = 9;
            }
        }
        minesLeft = mines;

        //Adjusting values of other tiles in the grid to reflect the number of mines surrounding them, maxmimum they should be able to reach is 8
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                if (gridMemory[i][j].val != 9)
                {
                    if (i != 0 && gridMemory[i - 1][j].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (j != 0 && gridMemory[i][j - 1].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (i != width - 1 && gridMemory[i + 1][j].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (j != height - 1 && gridMemory[i][j + 1].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (i != 0 && j != 0 && gridMemory[i - 1][j - 1].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (i != width - 1 && j != 0 && gridMemory[i + 1][j - 1].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (i != 0 && j != height - 1 && gridMemory[i - 1][j + 1].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                    if (i != width - 1 && j != height - 1 && gridMemory[i + 1][j + 1].val == 9)
                    {
                        gridMemory[i][j].val++;
                    }
                }
            }
        }
        cam.transform.position = new Vector3((float)width / 2 -0.5f, (float)height / 2 - 0.5f, -10);
    }

    private void Update()
    {
        if (tilesLeft == 0)
        {
            Lose();
        }
        MinesUI.text = "Mines: " + minesLeft.ToString();
    }

    public void RemoveTile()
    {
        tilesLeft--;
    }

    public void CheckSurrounding(int x, int y)
    {
        int count = 0;
        if (x != 0 && gridMemory[x - 1][y].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (y != 0 && gridMemory[x][y - 1].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (x != width - 1 && gridMemory[x + 1][y].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (y != height - 1 && gridMemory[x][y + 1].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (x != 0 && y != 0 && gridMemory[x - 1][y - 1].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (x != width - 1 && y != 0 && gridMemory[x + 1][y - 1].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (x != 0 && y != height - 1 && gridMemory[x - 1][y + 1].GetSprite() == TileSprites[10])
        {
            count++;
        }
        if (x != width - 1 && y != height - 1 && gridMemory[x + 1][y + 1].GetSprite() == TileSprites[10])
        {
            count++;
        }

        if (count == gridMemory[x][y].GetVal())
        {
            clearSurrounding(x, y);
        }
    }

    public void clearSurrounding(int x, int y)
    {
        if (x != 0 && gridMemory[x - 1][y].hidden && gridMemory[x - 1][y].GetSprite() != TileSprites[10])
        {
            gridMemory[x - 1][y].CheckSurrounding();
        }
        if (y != 0 && gridMemory[x][y - 1].hidden && gridMemory[x][y - 1].GetSprite() != TileSprites[10])
        {
            gridMemory[x][y - 1].CheckSurrounding();
        }
        if (x != width - 1 && gridMemory[x + 1][y].hidden && gridMemory[x + 1][y].GetSprite() != TileSprites[10])
        {
            gridMemory[x + 1][y].CheckSurrounding();
        }
        if (y != height - 1 && gridMemory[x][y + 1].hidden && gridMemory[x][y + 1].GetSprite() != TileSprites[10])
        {
            gridMemory[x][y + 1].CheckSurrounding();
        }
        if (x != 0 && y != 0 && gridMemory[x - 1][y - 1].hidden && gridMemory[x - 1][y - 1].GetSprite() != TileSprites[10])
        {
            gridMemory[x - 1][y - 1].CheckSurrounding();
        }
        if (x != width - 1 && y != 0 && gridMemory[x + 1][y - 1].hidden && gridMemory[x + 1][y - 1].GetSprite() != TileSprites[10])
        {
            gridMemory[x + 1][y - 1].CheckSurrounding();
        }
        if (x != 0 && y != height - 1 && gridMemory[x - 1][y + 1].hidden && gridMemory[x - 1][y + 1].GetSprite() != TileSprites[10])
        {
            gridMemory[x - 1][y + 1].CheckSurrounding();
        }
        if (x != width - 1 && y != height - 1 && gridMemory[x + 1][y + 1].hidden && gridMemory[x + 1][y + 1].GetSprite() != TileSprites[10])
        {
            gridMemory[x + 1][y + 1].CheckSurrounding();
        }
    }

    public void Lose()
    {
        for (int i = 0; i < width; i++) 
        { 
            for (int j = 0; j < height; j++)
            {
                if (gridMemory[i][j].hidden)
                {
                    gridMemory[i][j].hidden = false;
                }
            }
        }
    }

    public void Restart()
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                Destroy(gridMemory[i][j].gameObject);
            }
        }
        gridMemory.Clear();
        tilesLeft = width * height - mines;
        GenerateGrid();
    }

    public Sprite getSprite(int i)
    {
        return TileSprites[i];
    }

    public int GetWidth() { return width; }
    public int GetHeight() { return height; }
    public int GetMines() { return mines; }
}
