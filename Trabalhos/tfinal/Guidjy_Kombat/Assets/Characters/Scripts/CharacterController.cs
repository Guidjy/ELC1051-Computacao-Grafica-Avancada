using Characters.States;
using System.Linq;
using UnityEngine;
using UnityEngine.InputSystem;

namespace Characters.Movement
{
    public class CharacterController : MonoBehaviour
    {
        // Phisycs variables
        [SerializeField] private float movementSpeed = 2.7f;
        [SerializeField] private float jumpSpeed = 5f;
        [SerializeField] private LayerMask groundLayer;

        // Input actions
        [SerializeField] private InputActionReference stickMove;
        [SerializeField] private InputActionReference buttonPress;

        // Character components
        private Collider characterCollider;
        private Rigidbody rigidBody;
        private SpriteRenderer spriteRenderer;

        // Movement variables
        private Vector2 velocity;
        private Vector2 movementDirection;
        private MovementStates movementState;

        // Jump variables - TODO: allow continuous jumping
        private bool stickIsUp;
        private bool stickWasUp;
        private bool jumpRequested;
        private int remainingJumps;

        // Opponent variables
        private Transform opponentTransform;
        private float distanceFromOpponent;
        private bool shouldFlipHorizontaly;  // Used to control when the character should flip to face the opponent

        private void Awake()
        {
            characterCollider = GetComponent<Collider>();
            rigidBody = GetComponent<Rigidbody>();
            spriteRenderer = GetComponent<SpriteRenderer>();
        }

        void Start()
        {
            movementState = MovementStates.STANDING;
            stickIsUp = false;
            stickWasUp = false;

            opponentTransform = FindObjectsByType<CharacterController>().Where(opponent => opponent.transform != transform).First().transform;
        }

        void OnEnable()
        {
            stickMove.action.Enable();
            buttonPress.action.Enable();
        }

        void OnDisable()
        {
            stickMove.action.Disable();
            buttonPress.action.Disable();
        }

        void Update()
        {
            UpdateMovementDirection();

            CheckForJumpRequest();

            FaceOpponent();
        }

        private void FixedUpdate()
        {
            velocity = rigidBody.linearVelocity;

            if (IsGrounded())
            {
                SetGrounded();
                UpdateHorizontalVelocity();
            }

            if (jumpRequested && remainingJumps > 0)
            {
                Jump();
            }

            rigidBody.linearVelocity = velocity;
        }

        private void UpdateMovementDirection()
        {
            movementDirection = stickMove.action.ReadValue<Vector2>();
            // rounds to -1, 0, or 1 since that's what matters for traditional fighting game movement
            movementDirection.x = Mathf.Round(movementDirection.x);
            movementDirection.y = Mathf.Round(movementDirection.y);
        }

        private void CheckForJumpRequest()
        {
            stickIsUp = movementDirection.y > 0;
            if (stickIsUp && !stickWasUp) jumpRequested = true;
            stickWasUp = stickIsUp;
        }

        private void FaceOpponent()
        {
            float updatedDistanceFromOpponent = transform.position.x - opponentTransform.position.x;
            if (updatedDistanceFromOpponent * distanceFromOpponent < 0) shouldFlipHorizontaly = true;
            distanceFromOpponent = updatedDistanceFromOpponent;

            if (!shouldFlipHorizontaly)
            {
                return;
            }

            spriteRenderer.flipX = !spriteRenderer.flipX;
            shouldFlipHorizontaly = false;
        }

        private bool IsGrounded()
        {
            // Distance checked for colliders below the character
            const float groundCheckDistance = 0.05f;

            Bounds characterBounds = characterCollider.bounds;
            // Gets a point just below the character's feet
            Vector3 origin = new Vector3(characterBounds.center.x, characterBounds.min.y + 0.01f, characterBounds.center.z);
            return Physics.Raycast(origin, Vector3.down, groundCheckDistance + 0.01f, groundLayer);
        }

        private void SetGrounded()
        {
            movementState = MovementStates.STANDING;
            remainingJumps = 2;
        }

        private void UpdateHorizontalVelocity()
        {
            velocity.x = movementDirection.x * movementSpeed;
        }

        private void Jump()
        {
            velocity.x = movementDirection.x * movementSpeed;  // necessairy for being able to change directions when jumping
            velocity.y = jumpSpeed;

            movementState = MovementStates.JUMPING;
            jumpRequested = false;
            remainingJumps--;
        }
    }
}
