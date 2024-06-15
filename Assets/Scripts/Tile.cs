using System.Collections;
using System.Collections.Generic;
using System.Threading;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;

public class Tile : MonoBehaviour
{
    // Start is called before the first frame update

    [SerializeField] SpriteRenderer Renderer;
    [SerializeField] Sprite blankTile;
    [SerializeField] public bool hidden = true;
    [SerializeField] public int val = 0, x, y;
    [SerializeField] Sprite revealed;
    [SerializeField] Sprite mineSprite;
    [SerializeField] public GridManager grid;
    [SerializeField] private bool isClicked = false;
    [SerializeField] private bool isHovering = false;
    [SerializeField] private bool helper = false;
    void Start()
    {
        revealed = grid.getSprite(val);
    }

    // Update is called once per frame
    void Update()
    {
        if (!hidden)
        {
            Renderer.sprite = revealed;
            if (val == 9 && !grid.lost)
            {
                grid.Lose();
            }
        }
        if (isHovering && Input.GetMouseButtonDown(1))
        {
            if (Renderer.sprite == blankTile && !helper)
            {
                PlaceMine();
                print("placed");
                helper = true;
            }
            if (Renderer.sprite == mineSprite && !helper)
            {
                RemoveMine();
                print("removed");
                helper = true;
            }
            helper = false;
        }
    }
    public void OnMouseDown()
    {
        if (!isClicked)
        {
            isClicked = true;
        }
    }

    private void PlaceMine()
    {
        if (grid.minesLeft > 0)
        {
            grid.minesLeft--;
            Renderer.sprite = mineSprite;
        }
    }

    private void RemoveMine()
    {
        grid.minesLeft++;
        Renderer.sprite = blankTile;
    }

    public void OnMouseExit()
    {
        isClicked = false;
        isHovering = false;
    }

    public void OnMouseEnter()
    {
        isHovering = true;
    }

    public void OnMouseUp()
    {
        if (isClicked && !hidden && Renderer.sprite == revealed && val > 0)
        {
            grid.CheckSurrounding(x,y);
        }

        if (isClicked && hidden && Renderer.sprite == blankTile)
        {
            grid.RemoveTile();
            hidden = false;
            if (val == 0)
            {
                grid.clearSurrounding(x, y);
            }
            isClicked = false;
        }
    }

    public void CheckSurrounding()
    {
        hidden = false;
        grid.RemoveTile();
        if (val == 0)
        {
            grid.clearSurrounding(x, y);
        }
    }

    public bool IsHidden()
    {
        return hidden;
    }

    public Sprite GetSprite()
    {
        return Renderer.sprite;
    }

    public int GetVal()
    {
        return val;
    }
}